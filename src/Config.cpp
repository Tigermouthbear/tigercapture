//
// Created by Tigermouthbear on 8/18/20.
//

#include "Config.h"

#include "FileUtils.h"

Config::Config(const std::string& file) {
    this->file = file;

    json = FileUtils::readJSON(file, {
            { "minimize", true }
    });
}

void Config::Read() {
    minimize = json["minimize"];
}

void Config::Write() {
    json["minimize"] = minimize;

    FileUtils::writeJSON(file, json);
}

bool Config::shouldMinimize() const {
    return minimize;
}

void Config::setShouldMinimize(bool value) {
    minimize = value;
}


