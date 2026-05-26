#pragma once

#include "Config.hpp"

#include <map>
#include <string>
#include <vector>

struct Extreme {
    double value;
    std::string file;
};

struct Result {
    std::string sensor;
    std::string rule;
    std::string type;

    Extreme min;
    Extreme max;
};

class Parser {
private:
    const Config& config;

    std::map<std::string, Result> results;

    std::map<std::string, std::string> splitSensors(const std::string& text);
    std::vector<std::string> extractValues(const std::string& block, const std::string& pattern);

    double parseNumber(const std::vector<std::string>& values);
    bool parseBool(const Rule& rule, const std::vector<std::string>& values);
    double parseSpeed(const std::vector<std::string>& values);

public:
    Parser(const Config& config);

    std::vector<Result> parseFile(const std::string& text, const std::string& fileName);
    void merge(const Result& result);
    void printStats();

    static std::string formatSpeed(double value);
};