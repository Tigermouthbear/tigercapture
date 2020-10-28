//
// Created by Tigermouthbear on 10/26/20.
//

#include "MainWindow.h"

#include <QTimer>
#include <QLayout>
#include <QApplication>

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
    connect(fullButton, SIGNAL(released()), this, SLOT(handleFullScreenshot()));

    areaButton = new QPushButton("Area Screenshot", this);
    layout->addWidget(areaButton, 1, 0);
    connect(areaButton, SIGNAL(released()), this, SLOT(handleAreaScreenshot()));

    pinButton = new QPushButton("Pin Area", this);
    layout->addWidget(pinButton, 2, 0);
    connect(pinButton, SIGNAL(released()), this, SLOT(handlePinArea()));

    dragUploadButton = new QPushButton("Drag and Drop");
    layout->addWidget(dragUploadButton, 3, 0);
    connect(dragUploadButton, SIGNAL(released()), this, SLOT(dragUpload()));

    configButton = new QPushButton("Config", this);
    layout->addWidget(configButton, 4, 0);
    connect(configButton, SIGNAL(released()), this, SLOT(handleConfig()));

    layout->setColumnMinimumWidth(1, 300);
    uploadsExplorerWidget = new UploadsExplorerWidget(this, layout->columnMinimumWidth(1),layout->minimumSize().height() - 12);
    layout->addWidget(uploadsExplorerWidget, 0, 1, 5, 1);

    // set size
    move(tigerCapture->getConfig()->getX() - x(), tigerCapture->getConfig()->getY() - y());
    setFixedSize(450, 205);

    // start timer for updates
    auto* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(queryExplorerUpdate()));
    timer->start(1000);
}

void MainWindow::activateWindow() {
    setWindowState(Qt::WindowState::WindowActive);
    show();
}

// minimize, if needed, delay then actually fullscreen screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(fullScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else fullScreenshot();
    fullButton->setDown(false);
}

// minimize, if needed, delay then actually open area screenshot
void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else areaScreenshot();
    areaButton->setDown(false);
}

// minimize, if needed, delay then actually open pin region grabber
void MainWindow::handlePinArea() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, this, SLOT(pinArea()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else pinAreaImpl(tigerCapture);
    pinButton->setDown(false);
}

void MainWindow::handleConfig() {
    auto* configWidget = new ConfigWidget(tigerCapture);
    configWidget->move(x() + width() + 20, y());
    configWidget->show();
}

void MainWindow::fullScreenshot() {
    fullScreenshotImpl(tigerCapture);
}

void MainWindow::areaScreenshot() {
    areaScreenshotImpl(tigerCapture);
}

void MainWindow::pinArea() {
    pinAreaImpl(tigerCapture);
}

void MainWindow::dragUpload() {
    dragUploadImpl(tigerCapture);
}

void MainWindow::fullScreenshotImpl(TigerCapture* tigerCapture) {
    auto* screenshot = new Screenshot(tigerCapture);
    screenshot->take();
    auto future = screenshot->save();
}

AreaScreenshotGrabber* MainWindow::areaScreenshotImpl(TigerCapture* tigerCapture) {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(tigerCapture);
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
}

PinnedAreaGrabber* MainWindow::pinAreaImpl(TigerCapture* tigerCapture) {
    auto* pinnedAreaGrabber = new PinnedAreaGrabber(tigerCapture);
    pinnedAreaGrabber->show();
    return pinnedAreaGrabber;
}

DragUploadWidget* MainWindow::dragUploadImpl(TigerCapture* tigerCapture) {
    auto* dragUploadWidget = new DragUploadWidget(tigerCapture);
    dragUploadWidget->show();
    return dragUploadWidget;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    tigerCapture->getConfig()->setX(x());
    tigerCapture->getConfig()->setY(y());
    tigerCapture->getConfig()->write();
    tigerCapture->setWindowClosed();

    QMainWindow::closeEvent(event);
}

void MainWindow::queryExplorerUpdate() {
    if(tigerCapture->shouldUpdateUploadsExplorer()) uploadsExplorerWidget->updateUploads();
}
