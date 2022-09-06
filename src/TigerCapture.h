//
// Created by Tigermouthbear on 10/26/20.
//

#ifndef TIGERCAPTURE_TIGERCAPTURE_H
#define TIGERCAPTURE_TIGERCAPTURE_H

#include "TigerCapture.hpp"
#include "Config.h"
class OutputSource;
#include "widgets/gui/MainWindow.h"
#include "widgets/SystemTray.h"
#include "widgets/gui/UploadsExplorerWidget.h"

class TigerCapture: public QObject {
Q_OBJECT
private:
    Config* config;

    bool mainWindowOpen = false;
    MainWindow* mainWindow;

    SystemTray* systemTray;

    bool takingScreenshot = false;

    std::string resourcePath;

    OutputSource* output;

public:
    TigerCapture();
    ~TigerCapture();

    Config* getConfig();

    MainWindow* getMainWindow();
    void openWindow();
    void setWindowClosed();

    void setTakingScreenshot(bool value);

    SystemTray* getSystemTray();

    std::string getResource(const std::string& resource);

    OutputSource* getOutput();
    void setOutput(OutputSource* value);

    void upload(std::string file);

public slots:
    void fullScreenshot();
    void areaScreenshot();
    void pinArea();
    void dragUpload();
};

#include "OutputSource.h"

#endif //TIGERCAPTURE_TIGERCAPTURE_H
