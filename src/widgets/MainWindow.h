//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_MAINWINDOW_H
#define TIGERCAPTURE_MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QtWidgets/QSystemTrayIcon>
#include "../region/AreaScreenshotGrabber.h"
#include "../region/PinnedAreaGrabber.h"
#include "../Config.h"

class MainWindow: public QMainWindow {
Q_OBJECT
private:
    Config* config;

    QPushButton* fullButton;
    QPushButton* areaButton;
    QPushButton* pinButton;
    QPushButton* configButton;

    bool dontCloseYet = false;

    QSystemTrayIcon* icon;

public:
    MainWindow(QSystemTrayIcon* icon, Config* config);

    static void fullScreenshot(QSystemTrayIcon* icon, Config* config);
    static AreaScreenshotGrabber* areaScreenshot(QSystemTrayIcon* icon, Config* config);
    static PinnedAreaGrabber* pinArea(Config* config);

public slots:
    void fullScreenshot();
    void areaScreenshot();
    void pinArea();

    QSystemTrayIcon* getIcon();

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
