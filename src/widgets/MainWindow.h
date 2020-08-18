//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_MAINWINDOW_H
#define TIGERCAPTURE_MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
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

public:
    MainWindow(Config* config);

public slots:
    static void fullScreenshot();
    static AreaScreenshotGrabber* areaScreenshot();
    static PinnedAreaGrabber* pinArea();

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
