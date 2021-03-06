//
// Created by Tigermouthbear on 10/26/20.
//

#include "TigerCapture.h"

#include <string>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QtCore>

TigerCapture::TigerCapture() {
    config = new Config(this, (TC::Files::getConfigDirectory() + "/config.json").c_str());
    config->read();

    // find resource path and set system try icon
    std::string applicationDirPath = QDir::toNativeSeparators(QFileInfo(qApp->applicationDirPath()).absoluteFilePath()).toStdString();
    #if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    std::string dirs[] = { std::string(INSTALL_PREFIX) + "/share/tigercapture", "/usr/share/tigercapture", "/usr/local/share/tigercapture"/*, applicationDirPath*/ };
    #elif defined(Q_OS_WIN)
    std::string dirs[] = { applicationDirPath }
    #endif
    bool exists = false;
    std::string icon;
    for(const std::string& dir: dirs) {
        icon = dir + "/icons/icon-large.ico";
        if(QFile::exists(icon.c_str())) {
            resourcePath = dir;
            exists = true;
            break;
        }
    }
    // create system tray when resource path is found
    if(exists) systemTray = new SystemTray(this, icon);
    else systemTray = new SystemTray(this);
    systemTray->show();
}

Config* TigerCapture::getConfig() {
    return config;
}
MainWindow* TigerCapture::getMainWindow() {
    return mainWindow;
}

void TigerCapture::openWindow() {
    if(!mainWindowOpen) {
        mainWindow = new MainWindow(this);
        mainWindow->show();
        mainWindowOpen = true;
    }
}

void TigerCapture::setWindowClosed() {
    mainWindowOpen = false;
    delete mainWindow;
}

SystemTray* TigerCapture::getSystemTray() {
    return systemTray;
}

std::string TigerCapture::getResource(const std::string& resource) {
    return resourcePath.append("/").append(resource);
}

void TigerCapture::fullScreenshot() {
    auto* screenshot = new Screenshot(this);
    screenshot->take();
    auto future = screenshot->save();
}

void TigerCapture::areaScreenshot() {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(this);
    areaScreenshotGrabber->show();
}

void TigerCapture::pinArea() {
    auto* pinnedAreaGrabber = new PinnedAreaGrabber(this);
    pinnedAreaGrabber->show();
}

void TigerCapture::dragUpload() {
    auto* dragUploadWidget = new DragUploadWidget(this);
    dragUploadWidget->show();
}

TigerCapture::~TigerCapture() {
    delete mainWindow;
    delete systemTray;
}
