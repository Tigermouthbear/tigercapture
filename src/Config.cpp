//
// Created by Tigermouthbear on 10/26/20.
//

#include "Config.h"

#include "TigerCapture.hpp"

Config::Config(const std::string& configPath) {
    this->file = configPath;

    json = {
            {"minimize",  true},
            {"clipboard", true},
            {"uploader",  ""},
            {"x",         0},
            {"y",         0},
            {"delay",     0}
    };
    nlohmann::json read = TC::Files::readJSON(file);
    if(read != nullptr) json.merge_patch(read);
}

void Config::read() {
    minimize = json["minimize"];
    clipboard = json["clipboard"];
    setUploader(json["uploader"]);
    x = json["x"];
    y = json["y"];
    delay = json["delay"];
}

void Config::write() {
    json["minimize"] = minimize;
    json["clipboard"] = clipboard;
    json["uploader"] = uploaderLoc;
    json["x"] = x;
    json["y"] = y;
    json["delay"] = delay;

    TC::Files::writeJSON(file, json);
}

bool Config::shouldMinimize() const {
    return minimize;
}

void Config::setShouldMinimize(bool value) {
    minimize = value;
}

bool Config::shouldClipboard() const {
    return clipboard;
}

void Config::setShouldClipboard(bool value) {
    clipboard = value;
}

Uploader* Config::getUploader() {
    return uploader;
}

bool Config::setUploader(const std::string& value) {
    if(value == "None" || value.empty()) {
        if(uploader != nullptr) delete uploader;
        uploaderLoc = "";
        uploader = nullptr;
        return true;
    }

    if(value == "Imgur") {
        if(uploaderLoc == "Imgur") return true;

        // create imgur uploader
        Uploader* imgurUploader = new Uploader();
        imgurUploader->setURL("https://api.imgur.com/3/image");
        imgurUploader->setFileFormName("image");
        imgurUploader->setResponseRegex("$json:data.link$");
        imgurUploader->setType(Uploader::IMAGE_UPLOADER);
        imgurUploader->addHeaderData(std::pair("authorization", "Client-ID 123aea91ef7437d"));

        // set it
        uploaderLoc = "Imgur";
        delete uploader;
        uploader = imgurUploader;
    }


    Uploader* newUploader = Uploader::createFromJSON(TC::Files::getUploadersDirectory() + "/" + value);
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

int Config::getDelay() const {
    return delay;
}

void Config::setDelay(int delayIn) {
    this->delay = delayIn;
}


