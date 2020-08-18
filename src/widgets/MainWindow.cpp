//
// Created by Tigermouthbear on 8/16/20.
//

#include "MainWindow.h"

#include "ConfigWidget.h"
#include <QTimer>
#include <QLayout>

MainWindow::MainWindow(Config* config): QMainWindow() {
    this->config = config;

    setWindowTitle("TigerCapture");

    // create layout
    auto* widget = new QWidget(this);
    setCentralWidget(widget);
    auto* layout = new QGridLayout(widget);

    // initialize buttons
    fullButton = new QPushButton("Full Screenshot", this);
    fullButton->setDown(false);
    layout->addWidget(fullButton, 1, 0);
    connect(fullButton, SIGNAL (released()), this, SLOT (handleFullScreenshot()));

    areaButton = new QPushButton("Area Screenshot", this);
    areaButton->setDown(false);
    layout->addWidget(areaButton, 1, 1);
    connect(areaButton, SIGNAL (released()), this, SLOT (handleAreaScreenshot()));

    pinButton = new QPushButton("Pin Area", this);
    pinButton->setDown(false);
    layout->addWidget(pinButton, 2, 0);
    connect(pinButton, SIGNAL (released()), this, SLOT (handlePinArea()));

    configButton = new QPushButton("Config", this);
    configButton->setDown(false);
    layout->addWidget(configButton, 3, 0, 1, 2, Qt::AlignHCenter);
    connect(configButton, SIGNAL (released()), this, SLOT (handleConfig()));
}

void MainWindow::activateWindow() {
    setWindowState(Qt::WindowState::WindowActive);
}

// minimize, delay then actually take screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(fullScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else fullScreenshot();
    fullButton->setDown(false);
}

void MainWindow::fullScreenshot() {
    auto* screenshot = new Screenshot();
    screenshot->Take();
    screenshot->Save();
    delete screenshot;
}

void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else areaScreenshot();
    areaButton->setDown(false);
}

AreaScreenshotGrabber* MainWindow::areaScreenshot() {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber();
    areaScreenshotGrabber->show();
    return areaScreenshotGrabber;
}

void MainWindow::handlePinArea() {
    if(isActiveWindow() && config->shouldMinimize()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(pinArea()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else pinArea();
    pinButton->setDown(false);
}

PinnedAreaGrabber* MainWindow::pinArea() {
    auto* pinnedAreaGrabber = new PinnedAreaGrabber();
    pinnedAreaGrabber->show();
    return pinnedAreaGrabber;
}

void MainWindow::handleConfig() {
    auto* configWidget = new ConfigWidget(config);
    configWidget->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    config->Write();
}
