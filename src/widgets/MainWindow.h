//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_MAINWINDOW_H
#define TIGERCAPTURE_MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QLineEdit>
#include "../region/AreaScreenshotGrabber.h"
#include "../region/PinnedAreaGrabber.h"
#include "../Config.h"
#include "UploadsExplorerWidget.h"

class MainWindow: public QMainWindow {
Q_OBJECT
private:
    Config* config;

    QPushButton* fullButton;
    QPushButton* areaButton;
    QPushButton* pinButton;
    QPushButton* dragUploadButton;
    QPushButton* configButton;
    UploadsExplorerWidget* uploadsExplorerWidget;

public:
    MainWindow(Config* config);

    static void fullScreenshotImpl(Config* config);

    static AreaScreenshotGrabber* areaScreenshotImpl(Config* config);

    static PinnedAreaGrabber* pinArea(Config* config);

    static void dragUpload(Config* config);

public slots:

    void fullScreenshot();

    void areaScreenshot();

    void pinArea();

    void dragUpload();

private slots:

    void activateWindow();

    void handleFullScreenshot();

    void handleAreaScreenshot();

    void handlePinArea();

    void handleConfig();

protected:
    void closeEvent(QCloseEvent* event) override;
};


#endif //TIGERCAPTURE_MAINWINDOW_H
