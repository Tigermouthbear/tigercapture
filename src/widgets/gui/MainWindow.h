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
class Config;
class UploadsExplorerWidget;
class AreaScreenshotGrabber;
class PinnedAreaGrabber;
class Uploader;
#include "../../TigerCapture.h"
class TigerCapture;

#include "../../region/AreaScreenshotGrabber.h"
#include "../../region/PinnedAreaGrabber.h"
#include "../DragUploadWidget.h"

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

    void addWidgets(bool compact, bool refresh = false);
    bool isCompact(QSize size);

public:
    explicit MainWindow(TigerCapture* tigerCapture);
    void updateExplorer();

signals:
    void updateExplorerSignal();

public slots:
    void activateWindow();

private slots:
    void updateExplorerSlot();
    void handleFullScreenshot();
    void handleAreaScreenshot();
    void handlePinArea();
    void handleDragUpload();
    void handleConfig();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};


#endif //TIGERCAPTURE_MAINWINDOW_H
