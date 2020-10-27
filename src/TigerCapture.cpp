//
// Created by Tigermouthbear on 10/26/20.
//

#include "TigerCapture.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QtCore>
#include "FileUtils.h"

TigerCapture::TigerCapture() {
    config = new Config(FileUtils::getApplicationDirectory() + "/config.json");
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
        systemTray = new SystemTray(icon);
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

SystemTray* TigerCapture::getSystemTray() {
    return systemTray;
}

void TigerCapture::setUploadsExplorerWidget(UploadsExplorerWidget* uploadsExplorerWidget) {
    this->uploadsExplorerWidget = uploadsExplorerWidget;
}

void TigerCapture::updateUploadsExplorer() {
    if(uploadsExplorerWidget != nullptr) uploadsExplorerWidget->updateUploads();
}

std::string TigerCapture::getResource(const std::string& resource) {
    return resourcePath.append("/").append(resource);
}
