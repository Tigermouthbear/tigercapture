//
// Created by Tigermouthbear on 10/26/20.
//

#ifndef TIGERCAPTURE_MAINWINDOW_H
#define TIGERCAPTURE_MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGridLayout>

class MainWindow;
class UploadsExplorerWidget;
class AreaScreenshotGrabber;
class PinnedAreaGrabber;
#include "../TigerCapture.h"
class TigerCapture;

#include "../region/AreaScreenshotGrabber.h"
#include "../region/PinnedAreaGrabber.h"
#include "DragUploadWidget.h"

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
    explicit MainWindow(TigerCapture* tigerCapture);

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

    void queryExplorerUpdate();

protected:
    void closeEvent(QCloseEvent* event) override;
};


#endif //TIGERCAPTURE_MAINWINDOW_H
