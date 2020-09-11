#pragma once
//
// Created by Tigermouthbear on 8/18/20.
//

#ifndef TIGERCAPTURE_TIGERCAPTURE_H
#define TIGERCAPTURE_TIGERCAPTURE_H

class TigerCapture;

#include <string>
#include "json.hpp"
#include "Uploader.h"
#include "widgets/SystemTray.h"
#include "widgets/UploadsExplorerWidget.h"

class TigerCapture {
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

    SystemTray* systemTray = nullptr;

    UploadsExplorerWidget* uploadsExplorerWidget;

    std::string resourcePath;

public:
    explicit TigerCapture(const std::string& file);

    ~TigerCapture();

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

    SystemTray* getSystemTray();

    void setUploadsExplorerWidget(UploadsExplorerWidget* uploadsExplorerWidget);

    void updateUploadsExplorer();

    std::string getResource(const std::string& resource);

public slots:
    static void exit();
};


#endif //TIGERCAPTURE_TIGERCAPTURE_H
