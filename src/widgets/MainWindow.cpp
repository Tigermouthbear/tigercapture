//
// Created by Tigermouthbear on 8/16/20.
//

#include "MainWindow.h"

#include "ConfigWidget.h"
#include "DragUploadWidget.h"
#include <QTimer>
#include <QLayout>
#include <QApplication>

MainWindow::MainWindow(Config* config): QMainWindow() {
    this->config = config;

    setWindowTitle("TigerCapture");
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // create layout
    auto* widget = new QWidget(this);
    setCentralWidget(widget);
    auto* layout = new QGridLayout(widget);

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
    uploadsExplorerWidget = new UploadsExplorerWidget(this, layout->columnMinimumWidth(1), layout->minimumSize().height() - 12);
    layout->addWidget(uploadsExplorerWidget, 0, 1, 5, 1);

    move(config->getX() - x(), config->getY() - y());
}

void MainWindow::activateWindow() {
    setWindowState(Qt::WindowState::WindowActive);
}

// minimize, delay then actually fullscreen screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(fullScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else fullScreenshot();
    fullButton->setDown(false);
}

void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else areaScreenshot();
    areaButton->setDown(false);
}

void MainWindow::handlePinArea() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(pinArea()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else pinArea(config);
    pinButton->setDown(false);
}

PinnedAreaGrabber* MainWindow::pinArea(Config* config) {
    auto* pinnedAreaGrabber = new PinnedAreaGrabber(config);
    pinnedAreaGrabber->show();
    return pinnedAreaGrabber;
}

void MainWindow::dragUpload(Config *config) {
    auto* dragUploadWidget = new DragUploadWidget(config);
    dragUploadWidget->show();
}

void MainWindow::handleConfig() {
    auto* configWidget = new ConfigWidget(config);
    configWidget->move(x() + width() + 20, y());
    configWidget->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    config->setX(x());
    config->setY(y());
    config->write();
    QApplication::quit();
}

void MainWindow::fullScreenshot() {
    fullScreenshotImpl(config);
    uploadsExplorerWidget->updateUploads();
}

void MainWindow::areaScreenshot() {
    areaScreenshotImpl(config);
    uploadsExplorerWidget->updateUploads();
}

void MainWindow::fullScreenshotImpl(Config* config) {
    Screenshot screenshot = {config};
    screenshot.take();
    screenshot.save();
}

AreaScreenshotGrabber* MainWindow::areaScreenshotImpl(Config* config) {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(config);
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
}

void MainWindow::pinArea() {
    pinArea(config);
}

void MainWindow::dragUpload() {
    dragUpload(config);
}
