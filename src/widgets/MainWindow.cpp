//
// Created by Tigermouthbear on 10/26/20.
//

#include "MainWindow.h"

#include <QTimer>
#include <QLayout>
#include <QApplication>

#include "DragUploadWidget.h"
#include "ConfigWidget.h"

MainWindow::MainWindow(TigerCapture* tigerCapture): QMainWindow() {
    this->tigerCapture = tigerCapture;

    setWindowTitle("TigerCapture");
    setWindowFlags(Qt::WindowStaysOnTopHint);

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

    move(tigerCapture->getConfig()->getX() - x(), tigerCapture->getConfig()->getY() - y());
    setFixedSize(450, 205);
}

void MainWindow::activateWindow() {
    setWindowState(Qt::WindowState::WindowActive);
    show();
}

// minimize, delay then actually fullscreen screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(fullScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else fullScreenshot();
    fullButton->setDown(false);
}

void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else areaScreenshot();
    areaButton->setDown(false);
}

void MainWindow::handlePinArea() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
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
    tigerCapture->getConfig()->setX(x());
    tigerCapture->getConfig()->setY(y());
    tigerCapture->getConfig()->write();
    QApplication::exit();
}

void MainWindow::fullScreenshot() {
    fullScreenshotImpl(tigerCapture);
}

void MainWindow::areaScreenshot() {
    areaScreenshotImpl(tigerCapture);
}

void MainWindow::fullScreenshotImpl(TigerCapture* tigerCapture) {
    Screenshot screenshot = {tigerCapture};
    screenshot.take();
    screenshot.save();
    tigerCapture->updateUploadsExplorer();
}

AreaScreenshotGrabber* MainWindow::areaScreenshotImpl(TigerCapture* tigerCapture) {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(tigerCapture);
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
}

void MainWindow::pinArea() {
    pinArea(tigerCapture);
}

void MainWindow::dragUpload() {
    dragUpload(tigerCapture);
}
