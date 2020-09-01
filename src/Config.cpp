//
// Created by Tigermouthbear on 8/18/20.
//

#include "Config.h"
#include "FileUtils.h"

Config::Config(const std::string& file) {
    this->file = file;

    json = {
            { "minimize", true },
            { "clipboard", true },
            { "uploader", "" },
            { "x", 0 },
            { "y", 0 },
            { "delay", 0 }
    };
    nlohmann::json read = FileUtils::readJSON(file);
    if(read != nullptr) json.merge_patch(read);

    // set system try icon
    systemTrayIcon = new QSystemTrayIcon();
    systemTrayIcon->setIcon(QIcon("../icons/icon.ico"));
    systemTrayIcon->show();
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

    FileUtils::writeJSON(file, json);
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

QSystemTrayIcon* Config::getSystemTrayIcon() {
    return systemTrayIcon;
}

int Config::getDelay() const {
    return delay;
}

void Config::setDelay(int delayIn) {
    this->delay = delayIn;
}
