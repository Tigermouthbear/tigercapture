//
// Created by Tigermouthbear on 8/16/20.
//

#include "MainWindow.h"

#include "ConfigWidget.h"
#include "DragUploadWidget.h"
#include <QTimer>
#include <QLayout>
#include <QApplication>

MainWindow::MainWindow(TigerCapture* tigerCapture): QMainWindow() {
    this->tigerCapture = tigerCapture;

    setWindowTitle("TigerCapture");
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);

    // create layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QGridLayout(centralWidget);

    // initialize buttons
    fullButton = new QPushButton("Full Screenshot", this);
    layout->addWidget(fullButton, 0, 0);
    connect(fullButton, SIGNAL (released()), this, SLOT (handleFullScreenshot()));

    areaButton = new QPushButton("Area Screenshot", this);
    layout->addWidget(areaButton, 1, 0);
    connect(areaButton, SIGNAL (released()), this, SLOT (handleAreaScreenshot()));

    pinButton = new QPushButton("Pin Area", this);
    layout->addWidget(pinButton, 2, 0);
    connect(pinButton, SIGNAL (released()), this, SLOT (handlePinArea()));

    dragUploadButton = new QPushButton("Drag and Drop");
    layout->addWidget(dragUploadButton, 3, 0);
    connect(dragUploadButton, SIGNAL (released()), this, SLOT (dragUpload()));

    configButton = new QPushButton("Config", this);
    layout->addWidget(configButton, 4, 0);
    connect(configButton, SIGNAL (released()), this, SLOT (handleConfig()));

    layout->setColumnMinimumWidth(1, 300);
    uploadsExplorerWidget = new UploadsExplorerWidget(this, layout->columnMinimumWidth(1),layout->minimumSize().height() - 12);
    layout->addWidget(uploadsExplorerWidget, 0, 1, 5, 1);
    tigerCapture->setUploadsExplorerWidget(uploadsExplorerWidget);

    move(tigerCapture->getX() - x(), tigerCapture->getY() - y());
}

void MainWindow::activateWindow() {
    setWindowState(Qt::WindowState::WindowActive);
    show();
}

// minimize, delay then actually fullscreen screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && tigerCapture->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(fullScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else fullScreenshot();
    fullButton->setDown(false);
}

void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && tigerCapture->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else areaScreenshot();
    areaButton->setDown(false);
}

void MainWindow::handlePinArea() {
    if(isActiveWindow() && tigerCapture->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(pinArea()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else pinArea(tigerCapture);
    pinButton->setDown(false);
}

PinnedAreaGrabber* MainWindow::pinArea(TigerCapture* tigerCapture) {
    auto* pinnedAreaGrabber = new PinnedAreaGrabber(tigerCapture);
    pinnedAreaGrabber->show();
    return pinnedAreaGrabber;
}

void MainWindow::dragUpload(TigerCapture* tigerCapture) {
    auto* dragUploadWidget = new DragUploadWidget(tigerCapture);
    dragUploadWidget->show();
}

void MainWindow::handleConfig() {
    auto* configWidget = new ConfigWidget(tigerCapture);
    configWidget->move(x() + width() + 20, y());
    configWidget->show();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    tigerCapture->setX(x());
    tigerCapture->setY(y());
    tigerCapture->write();
    QApplication::quit();
}

void MainWindow::fullScreenshot() {
    fullScreenshotImpl(tigerCapture);
}

void MainWindow::areaScreenshot() {
    areaScreenshotImpl(tigerCapture);
}

void MainWindow::fullScreenshotImpl(TigerCapture* config) {
    Screenshot screenshot = {config};
    screenshot.take();
    screenshot.save();
    config->updateUploadsExplorer();
}

AreaScreenshotGrabber* MainWindow::areaScreenshotImpl(TigerCapture* config) {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(config);
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
}

void MainWindow::pinArea() {
    pinArea(tigerCapture);
}

void MainWindow::dragUpload() {
    dragUpload(tigerCapture);
}

MainWindow::~MainWindow() {
    delete tigerCapture;
    delete fullButton;
    delete areaButton;
    delete pinButton;
    delete dragUploadButton;
    delete configButton;
    delete uploadsExplorerWidget;
    delete centralWidget;
    delete layout;
}
