//
// Created by Tigermouthbear on 10/26/20.
//

#ifndef TIGERCAPTURE_TIGERCAPTURE_H
#define TIGERCAPTURE_TIGERCAPTURE_H

#include "TigerCapture.hpp"
#include "Config.h"
#include "widgets/MainWindow.h"
#include "widgets/SystemTray.h"
#include "widgets/UploadsExplorerWidget.h"

class TigerCapture {
private:
    Config* config;
    MainWindow* mainWindow = nullptr;
    SystemTray* systemTray = nullptr;

    bool shouldUpdateExplorer = false;

    std::string resourcePath;

public:
    TigerCapture();

    Config* getConfig();

    MainWindow* getMainWindow();
    void openWindow();
    void setWindowClosed();

    SystemTray* getSystemTray();

    void updateUploadsExplorer();
    bool shouldUpdateUploadsExplorer() const;

    std::string getResource(const std::string& resource);

    void fullScreenshot();
    void areaScreenshot();
    void pinArea();
    void dragUpload();
};


#endif //TIGERCAPTURE_TIGERCAPTURE_H
