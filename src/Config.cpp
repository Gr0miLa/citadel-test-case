#include "Config.hpp"

Config parseConfig(const nlohmann::json& jsonConfig) {
    Config config;

    for (const auto& sensorJson : jsonConfig["sensors"]) {
        Sensor sensor;

        sensor.name = sensorJson["name"];
        sensor.rule = sensorJson["rule"];

        config.sensors.push_back(sensor);
    }

    for (const auto& ruleJson : jsonConfig["rules"]) {
        Rule rule;

        rule.name = ruleJson["name"];
        rule.type = ruleJson["type"];
        rule.rule = ruleJson["rule"];

        if (rule.type == "bool") {
            rule.trueValue = ruleJson["true"];
            rule.falseValue = ruleJson["false"];
        }

        config.rules.push_back(rule);
    }

    for (const auto& extractorJson : jsonConfig["extractors"]) {
        Extractor extractor;

        extractor.sensor = extractorJson["sensor"];

        for (const auto& rule : extractorJson["rules"]) {
            extractor.rules.push_back(rule);
        }

        config.extractors.push_back(extractor);
    }

    return config;
}