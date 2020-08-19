//
// Created by Tigermouthbear on 8/18/20.
//

#include "Config.h"

#include "FileUtils.h"

Config::Config(const std::string& file) {
    this->file = file;

    json = {
            { "minimize", true },
            { "upload", true }
    };
    try {
        nlohmann::json read = FileUtils::readJSON(file);
        if(read != nullptr) json = read;
    } catch(nlohmann::json::parse_error ignored) {
    }

    std::cout << json["upload"] << std::endl;
}

void Config::Read() {
    minimize = json["minimize"];
    upload = json["upload"];
    uploader = Uploader::createFromJSON(FileUtils::getApplicationDirectory() + "/uploader.json");
}

void Config::Write() {
    json["minimize"] = minimize;
    json["upload"] = upload;

    FileUtils::writeJSON(file, json);
}

bool Config::shouldMinimize() const {
    return minimize;
}

void Config::setShouldMinimize(bool value) {
    minimize = value;
}

bool Config::shouldUpload() const {
    return upload;
}

void Config::setShouldUpload(bool value) {
    upload = value;
}

Uploader* Config::getUploader() {
    return uploader;
}
