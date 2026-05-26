#include "Parser.hpp"

#include <format>
#include <iostream>
#include <regex>


Parser::Parser(const Config& config) : config(config) {}

std::vector<Result> Parser::parseFile(const std::string& text, const std::string& fileName) {
    std::vector<Result> results;

    auto sensorBlocks = splitSensors(text);

    for (const auto& [sensorName, block] : sensorBlocks) {
        for (const auto& extractor : config.extractors) {
            if (extractor.sensor != sensorName) {
                continue;
            }

            for (const auto& ruleName : extractor.rules) {
                for (const auto& rule : config.rules) {
                    if (rule.name != ruleName) {
                        continue;
                    }

                    auto values = extractValues(block, rule.rule);
                    if (values.empty()) {
                        std::cerr << "Warning: No values extracted for rule " << rule.name << "\n";
                        continue;
                    }
                    double value = 0;

                    try {
                        if (rule.type == "value") {
                            value = parseNumber(values);
                        }
                        else if (rule.type == "speed") {
                            value = parseSpeed(values);
                        }
                        else if (rule.type == "bool") {
                            value = parseBool(rule, values) ? 1.0 : 0.0;
                        }
                    }
                    catch (...) {
                        continue;
                    }

                    Result result;

                    result.sensor = sensorName;
                    result.rule = rule.name;
                    result.type = rule.type;

                    result.min = { value, fileName };
                    result.max = { value, fileName };

                    results.push_back(result);
                }
            }
        }
    }

    return results;
}

std::map<std::string, std::string> Parser::splitSensors(const std::string& text) {
    std::map<std::string, std::string> result;

    for (size_t i = 0; i < config.sensors.size(); ++i) {
        const Sensor& sensor = config.sensors[i];

        size_t start = text.find(sensor.rule);

        if (start == std::string::npos) {
            continue;
        }

        size_t end = text.size();

        if (i + 1 < config.sensors.size()) {
            const Sensor& nextSensor = config.sensors[i + 1];

            size_t nextPos = text.find(
                nextSensor.rule,
                start + sensor.rule.size()
            );

            if (nextPos != std::string::npos) {
                end = nextPos;
            }
        }

        result[sensor.name] = text.substr(start, end - start);
    }

    return result;
}

std::vector<std::string> Parser::extractValues(const std::string& block, const std::string& pattern) {
    std::vector<std::string> values;
    std::regex regex(pattern);
    std::smatch match;

    if (std::regex_search(block, match, regex)) {
        for (size_t i = 1; i < match.size(); ++i) {
            values.push_back(match[i]);
        }
    }

    return values;
}

double Parser::parseNumber(const std::vector<std::string>& values) {
    try {
        return std::stod(values[0]);
    }
    catch (...) {
        std::cerr << "Warning: Failed to parse number from " << values[0] << "\n";
        return 0.0;
    }
}

bool Parser::parseBool(const Rule& rule, const std::vector<std::string>& values) {
    return values[0] == rule.trueValue;
}

double Parser::parseSpeed(const std::vector<std::string>& values) {
    if (values.size() < 2) {
        std::cerr << "Warning: Missing speed unit\n";
        return 0.0;
    }

    double value = 0;

    try {
        value = std::stod(values[0]);
    }
    catch (...) {
        std::cerr << "Warning: Failed to parse speed from " << values[0] << "\n";
        return 0.0;
    }

    std::string unit = values[1];

    if (unit == "Kbit") {
        return value * 1e3;
    }
    else if (unit == "Mbit") {
        return value * 1e6;
    }
    else if (unit == "Gbit") {
        return value * 1e9;
    }

    return value;
}

std::string Parser::formatSpeed(double value) {
    if (value >= 1e9) {
        return std::format("{:g} Gbit/s", value / 1e9);
    }
    if (value >= 1e6) {
        return std::format("{:g} Mbit/s", value / 1e6);
    }
    if (value >= 1e3) {
        return std::format("{:g} Kbit/s", value / 1e3);
    }
    return std::format("{:g} bit/s", value);
}

void Parser::merge(const Result& result) {
    std::string key = result.sensor + "|" + result.rule;

    auto it = results.find(key);

    if (it == results.end()) {
        results[key] = result;
        return;
    }

    Result& current = it->second;

    if (result.min.value < current.min.value) {
        current.min = result.min;
    }

    if (result.max.value > current.max.value) {
        current.max = result.max;
    }
}

void Parser::printStats() {
    for (const auto& sensor : config.sensors) {
        std::cout << sensor.name << ":\n";

        for (const auto& extractor : config.extractors) {
            if (extractor.sensor != sensor.name) {
                continue;
            }

            for (const auto& ruleName : extractor.rules) {
                std::string key = sensor.name + "|" + ruleName;

                auto it = results.find(key);
                if (it == results.end()) {
                    continue;
                }

                const Result& result = it->second;
                std::string maxValue;
                std::string minValue;

                if (result.type == "speed") {
                    maxValue = formatSpeed(result.max.value);
                    minValue = formatSpeed(result.min.value);
                }
                else if (result.type == "bool") {
                    for (const auto& rule : config.rules) {
                        if (rule.name != result.rule) {
                            continue;
                        }

                        maxValue = static_cast<bool>(result.max.value) ? rule.trueValue : rule.falseValue;
                        minValue = static_cast<bool>(result.min.value) ? rule.trueValue : rule.falseValue;
                    }
                }
                else {
                    maxValue = std::format("{:g}", result.max.value);
                    minValue = std::format("{:g}", result.min.value);
                }

                std::cout << std::format( "    {}: max={}({}), min={}({})\n",
                    result.rule, maxValue, result.max.file, minValue, result.min.file);
            }
        }

        std::cout << "\n";
    }
}