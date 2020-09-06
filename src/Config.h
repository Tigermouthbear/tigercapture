#pragma once
//
// Created by Tigermouthbear on 8/18/20.
//

#ifndef TIGERCAPTURE_CONFIG_H
#define TIGERCAPTURE_CONFIG_H

class Config;

#include <string>
#include <QtWidgets/QSystemTrayIcon>
#include "json.hpp"
#include "Uploader.h"

class Config {
private:
    std::string file;
    nlohmann::json json;

    bool minimize{};

    bool clipboard{};

    std::string uploaderLoc;
    Uploader* uploader{};

    int x;
    int y;

    int delay;

    QSystemTrayIcon* systemTrayIcon;

public:
    Config(const std::string& file);

    void read();
    void write();

    bool shouldMinimize() const;
    void setShouldMinimize(bool value);

    bool shouldClipboard() const;
    void setShouldClipboard(bool value);

    Uploader* getUploader();
    bool setUploader(const std::string& file);
    std::string getUploaderLoc();

    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);

    int getDelay() const;
    void setDelay(int delayIn);

    QSystemTrayIcon* getSystemTrayIcon();
};


#endif //TIGERCAPTURE_CONFIG_H
