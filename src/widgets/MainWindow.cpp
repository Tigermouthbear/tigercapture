//
// Created by Tigermouthbear on 8/16/20.
//

#include "MainWindow.h"

#include "ConfigWidget.h"
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
    layout->addWidget(fullButton, 1, 0);
    connect(fullButton, SIGNAL (released()), this, SLOT (handleFullScreenshot()));

    areaButton = new QPushButton("Area Screenshot", this);
    layout->addWidget(areaButton, 1, 1);
    connect(areaButton, SIGNAL (released()), this, SLOT (handleAreaScreenshot()));

    pinButton = new QPushButton("Pin Area", this);
    layout->addWidget(pinButton, 2, 0);
    connect(pinButton, SIGNAL (released()), this, SLOT (handlePinArea()));

    configButton = new QPushButton("Config", this);
    layout->addWidget(configButton, 3, 0, 1, 2, Qt::AlignHCenter);
    connect(configButton, SIGNAL (released()), this, SLOT (handleConfig()));

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

void MainWindow::handleConfig() {
    auto* configWidget = new ConfigWidget(config);
    configWidget->move(x() + width(), y());
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
}

void MainWindow::areaScreenshot() {
    areaScreenshotImpl(config);
}

void MainWindow::fullScreenshotImpl(Config* config) {
    Screenshot screenshot = {config};
    screenshot.take();
    screenshot.save(); // save sync
}

AreaScreenshotGrabber* MainWindow::areaScreenshotImpl(Config* config) {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(config);
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
}

void MainWindow::pinArea() {
    pinArea(config);
}
