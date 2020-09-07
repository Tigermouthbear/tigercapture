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
#include "../TigerCapture.h"
#include "UploadsExplorerWidget.h"

class MainWindow: public QMainWindow {
Q_OBJECT
private:
    TigerCapture* tigerCapture;

    QWidget* centralWidget;
    QGridLayout* layout;

    QPushButton* fullButton;
    QPushButton* areaButton;
    QPushButton* pinButton;
    QPushButton* dragUploadButton;
    QPushButton* configButton;
    UploadsExplorerWidget* uploadsExplorerWidget;

public:
    MainWindow(TigerCapture* tigerCapture);

    ~MainWindow() override;

    static void fullScreenshotImpl(TigerCapture* tigerCapture);

    static AreaScreenshotGrabber* areaScreenshotImpl(TigerCapture* tigerCapture);

    static PinnedAreaGrabber* pinArea(TigerCapture* tigerCapture);

    static void dragUpload(TigerCapture* tigerCapture);

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
