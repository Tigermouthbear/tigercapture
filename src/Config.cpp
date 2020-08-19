//
// Created by Tigermouthbear on 8/18/20.
//

#include "Config.h"

#include "FileUtils.h"

Config::Config(const std::string& file) {
    this->file = file;

    json = {
            { "minimize", true },
            { "uploader", "" }
    };
    nlohmann::json read = FileUtils::readJSON(file);
    if(read != nullptr) json.merge_patch(read);
}

void Config::read() {
    minimize = json["minimize"];
    setUploader(json["uploader"]);
    if (json.contains("x")) {
        x = json["x"];
        y = json["y"];
    }
}

void Config::write() {
    json["minimize"] = minimize;
    json["uploader"] = uploaderLoc;
    json["x"] = x;
    json["y"] = y;

    FileUtils::writeJSON(file, json);
}

bool Config::shouldMinimize() const {
    return minimize;
}

void Config::setShouldMinimize(bool value) {
    minimize = value;
}

Uploader* Config::getUploader() {
    return uploader;
}

bool Config::setUploader(const std::string& value) {
    if(value == "None" || value.empty()) {
        uploaderLoc = "";
        uploader = nullptr;
        return true;
    }

    Uploader* newUploader = Uploader::createFromJSON(FileUtils::getUploadersDirectory() + "/" + value);
    if(newUploader == nullptr) return false;

    uploaderLoc = value;
    delete uploader;
    uploader = newUploader;
    return true;
}

std::string Config::getUploaderLoc() {
    return uploaderLoc;
}

int Config::getX() const {
    return x;
}
void Config::setX(int xIn) {
    x = xIn;
}

int Config::getY() const {
    return y;
}
void Config::setY(int yIn) {
    y = yIn;
}
