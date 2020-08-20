//
// Created by Tigermouthbear on 8/16/20.
//

#include "MainWindow.h"

#include "ConfigWidget.h"
#include <QTimer>
#include <QLayout>
#include <zconf.h>

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
    } else fullScreenshot(config);
    fullButton->setDown(false);
}

void MainWindow::fullScreenshot(Config* config) {
    Screenshot screenshot = {config};
    screenshot.take();
    screenshot.save(); // save sync
}

void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else areaScreenshot(config);
    areaButton->setDown(false);
}

AreaScreenshotGrabber* MainWindow::areaScreenshot(Config* config) {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber(config);
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
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
    configWidget->move(x() + width() / 2, y() + height() / 2);
    configWidget->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    config->write();
    while (dontCloseYet) {
        usleep(0);
    }
}

void MainWindow::fullScreenshot() {
    fullScreenshot(config);
}


void MainWindow::areaScreenshot() {
    areaScreenshot(config);
}

void MainWindow::pinArea() {
    pinArea(config);
}
