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
    std::string dirs[] = { applicationDirPath };
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

    output = new OutputSource();
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
    } else {
        mainWindow->show();
        mainWindow->activateWindow();
        mainWindow->raise();
    }
}

void TigerCapture::setWindowClosed() {
    mainWindowOpen = false;
    delete mainWindow;
}

void TigerCapture::setTakingScreenshot(bool value) {
  this->takingScreenshot = value;
}

SystemTray* TigerCapture::getSystemTray() {
    return systemTray;
}

OutputSource* TigerCapture::getOutput() {
    return output;
}

void TigerCapture::setOutput(OutputSource* value) {
    delete output;
    output = value;
}

std::string TigerCapture::getResource(const std::string& resource) {
    return resourcePath.append("/").append(resource);
}

static bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

void TigerCapture::upload(std::string file) {
    if(config->getUploader() == nullptr) {
        output->write("No uploader selected! Please check your config.\n");
        output->flush();
        return;
    }

    // make sure it can upload
    bool upload;
    if(endsWith(file, ".txt")) {
        upload = config->getUploader()->check(Uploader::TEXT_UPLOADER);
    } else if(endsWith(file, ".png") || endsWith(file, ".jpg") || endsWith(file, ".jpeg") || endsWith(file, ".gif")) {
        upload = config->getUploader()->check(Uploader::IMAGE_UPLOADER);
    } else {
        upload = config->getUploader()->check(Uploader::FILE_UPLOADER);
    }
    if(!upload) {
        output->write("Current uploader does not accept this file type!\n");
        output->flush();
    }

    // discard future, just let this run in the background
    std::async([=]() {
        // clear so user doesnt accidentally paste something else while waiting for the image to upload
        TC::Clipboard::clearClipboard();

        // actually upload
        std::string res = config->getUploader()->upload(file);
        if(res.empty()) return;

        // copy response
        TC::Clipboard::copyToClipboard(res);
    });
}

void TigerCapture::fullScreenshot() {
    auto* screenshot = new Screenshot(this);
    screenshot->take();
    screenshot->save();
}

void TigerCapture::areaScreenshot() {
    if(!this->takingScreenshot) (new AreaScreenshotGrabber(this))->open();
}

void TigerCapture::pinArea() {
    if(!this->takingScreenshot) (new PinnedAreaGrabber(this))->open();
}

void TigerCapture::dragUpload() {
    auto* dragUploadWidget = new DragUploadWidget(this);
    dragUploadWidget->show();
}

TigerCapture::~TigerCapture() {
    if(mainWindowOpen) delete mainWindow;
    delete output;
    delete systemTray;
    delete config;
}
