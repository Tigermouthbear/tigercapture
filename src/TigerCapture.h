//
// Created by Tigermouthbear on 10/26/20.
//

#ifndef TIGERCAPTURE_TIGERCAPTURE_H
#define TIGERCAPTURE_TIGERCAPTURE_H

#include "Config.h"

#include "widgets/MainWindow.h"
#include "widgets/SystemTray.h"
#include "widgets/UploadsExplorerWidget.h"

class TigerCapture {
private:
    Config* config;
    MainWindow* mainWindow = nullptr;
    SystemTray* systemTray = nullptr;

    UploadsExplorerWidget* uploadsExplorerWidget;

    std::string resourcePath;

public:
    TigerCapture();

    Config* getConfig();

    MainWindow* getMainWindow();
    void openWindow();

    SystemTray* getSystemTray();

    void setUploadsExplorerWidget(UploadsExplorerWidget* uploadsExplorerWidget);
    void updateUploadsExplorer();

    std::string getResource(const std::string& resource);
};


#endif //TIGERCAPTURE_TIGERCAPTURE_H
