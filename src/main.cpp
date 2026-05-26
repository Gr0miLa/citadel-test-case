#include "Config.hpp"
#include "FileReader.hpp"
#include "Parser.hpp"

#include <filesystem>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: parser <files>\n";
            return 1;
        }

        std::string configText = FileReader::read("../config/config.json");
        json jsonConfig = json::parse(configText);
        Config config = parseConfig(jsonConfig);

        Parser parser(config);

        std::vector<std::thread> threads;
        std::mutex mutex;
        size_t maxThreads = std::thread::hardware_concurrency();

        // std::cout << "Threads count: " << maxThreads << "\n";

        for (int i = 1; i < argc; ++i) {
            std::string filePath = argv[i];

            if (!filePath.ends_with(".txt")) {
                continue;
            }

            if (threads.size() >= maxThreads) {
                threads.front().join();
                threads.erase(threads.begin());
            }

            threads.emplace_back(
                [&parser, &mutex, filePath]() {
                    std::string text = FileReader::read(filePath);

                    std::string fileName = std::filesystem::path(filePath)
                        .filename()
                        .string();

                    auto results = parser.parseFile(text, fileName);

                    std::lock_guard lock(mutex);

                    for (const auto& result : results) {
                        parser.merge(result);
                    }
                }
            );
        }

        for (auto& thread : threads) {
            thread.join();
        }

        parser.printStats();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}