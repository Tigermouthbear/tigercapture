//
// Created by Tigermouthbear on 10/26/20.
//

#include "TigerCapture.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QtCore>

TigerCapture::TigerCapture() {
    config = new Config(TC::Files::getConfigDirectory() + "/config.json");
    config->read();

    // find resource path and set system try icon
    std::string applicationDirPath = QDir::toNativeSeparators(QFileInfo(qApp->applicationDirPath()).absoluteFilePath()).toStdString();
    #if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    std::string dirs[] = { std::string(INSTALL_PREFIX) + "/share/tigercapture", "/usr/share/tigercapture", "/usr/local/share/tigercapture"/*, applicationDirPath*/ };
    #elif defined(Q_OS_WIN)
    std::string dirs[] = { applicationDirPath }
    #endif
    for(const std::string& dir: dirs) {
        std::string icon = dir + "/icons/icon-large.ico";
        if(!QFile::exists(icon.c_str())) continue;

        // create system tray when resource path is found
        systemTray = new SystemTray(this, icon);
        systemTray->show();

        resourcePath = dir;
        break;
    }
}

Config* TigerCapture::getConfig() {
    return config;
}
MainWindow* TigerCapture::getMainWindow() {
    return mainWindow;
}

void TigerCapture::openWindow() {
    if(mainWindow == nullptr) {
        mainWindow = new MainWindow(this);
        mainWindow->show();
    }
}

void TigerCapture::setWindowClosed() {
    mainWindow = nullptr;
}

SystemTray* TigerCapture::getSystemTray() {
    return systemTray;
}

void TigerCapture::updateUploadsExplorer() {
    shouldUpdateExplorer = true;
}

bool TigerCapture::shouldUpdateUploadsExplorer() const {
    return shouldUpdateExplorer;
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
