#pragma once

#include <json.hpp>

#include <string>
#include <vector>

struct Sensor {
    std::string name;
    std::string rule;
};

struct Rule {
    std::string name;
    std::string type;
    std::string rule;

    std::string trueValue;
    std::string falseValue;
};

struct Extractor {
    std::string sensor;
    std::vector<std::string> rules;
};

struct Config {
    std::vector<Sensor> sensors;
    std::vector<Rule> rules;
    std::vector<Extractor> extractors;
};

Config parseConfig(const nlohmann::json& jsonConfig);
