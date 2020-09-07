//
// Created by Tigermouthbear on 8/18/20.
//

#include "TigerCapture.h"
#include "FileUtils.h"

TigerCapture::TigerCapture(const std::string& file) {
    this->file = file;

    json = {
            {"minimize",  true},
            {"clipboard", true},
            {"uploader",  ""},
            {"x",         0},
            {"y",         0},
            {"delay",     0}
    };
    nlohmann::json read = FileUtils::readJSON(file);
    if(read != nullptr) json.merge_patch(read);

    // set system try icon
    systemTrayIcon = new QSystemTrayIcon();
    systemTrayIcon->setIcon(QIcon("../icons/icon.ico"));
    systemTrayIcon->show();
}

void TigerCapture::read() {
    minimize = json["minimize"];
    clipboard = json["clipboard"];
    setUploader(json["uploader"]);
    x = json["x"];
    y = json["y"];
    delay = json["delay"];
}

void TigerCapture::write() {
    json["minimize"] = minimize;
    json["clipboard"] = clipboard;
    json["uploader"] = uploaderLoc;
    json["x"] = x;
    json["y"] = y;
    json["delay"] = delay;

    FileUtils::writeJSON(file, json);
}

bool TigerCapture::shouldMinimize() const {
    return minimize;
}

void TigerCapture::setShouldMinimize(bool value) {
    minimize = value;
}

bool TigerCapture::shouldClipboard() const {
    return clipboard;
}

void TigerCapture::setShouldClipboard(bool value) {
    clipboard = value;
}

Uploader* TigerCapture::getUploader() {
    return uploader;
}

bool TigerCapture::setUploader(const std::string& value) {
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

std::string TigerCapture::getUploaderLoc() {
    return uploaderLoc;
}

int TigerCapture::getX() const {
    return x;
}

void TigerCapture::setX(int xIn) {
    x = xIn;
}

int TigerCapture::getY() const {
    return y;
}

void TigerCapture::setY(int yIn) {
    y = yIn;
}

QSystemTrayIcon* TigerCapture::getSystemTrayIcon() {
    return systemTrayIcon;
}

int TigerCapture::getDelay() const {
    return delay;
}

void TigerCapture::setDelay(int delayIn) {
    this->delay = delayIn;
}

void TigerCapture::setUploadsExplorerWidget(UploadsExplorerWidget* uploadsExplorerWidget) {
    this->uploadsExplorerWidget = uploadsExplorerWidget;
}

void TigerCapture::updateUploadsExplorer() {
    if(uploadsExplorerWidget != nullptr) uploadsExplorerWidget->updateUploads();
}

TigerCapture::~TigerCapture() {
    delete systemTrayIcon;
}
