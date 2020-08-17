//
// Created by Tigermouthbear on 8/16/20.
//

#include "MainWindow.h"
#include "Screenshot.h"
#include "region/AreaScreenshotGrabber.h"
#include "region/PinnedAreaGrabber.h"

#include <QTimer>
#include <QLayout>

// creates the window
void MainWindow::Create() {
    // set window details
    resize(250, 150);
    setWindowTitle("TigerCapture");

    // create layout
    auto* widget = new QWidget(this);
    setCentralWidget(widget);
    auto* layout = new QGridLayout(widget);

    // initialize checkbox
    shouldMinimizeButton = new QCheckBox("Close window on capture", this);
    shouldMinimizeButton->setChecked(true);
    layout->addWidget(shouldMinimizeButton, 0, 0, 2, 0, Qt::AlignHCenter);

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

    // show window
    show();
}

// minimize, delay then actually take screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && shouldMinimizeButton->isChecked()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(fullScreenShot()));
    } else fullScreenShot();
    fullButton->setDown(false);
}

// take screenshot then set window back to active
void MainWindow::fullScreenShot() {
    Screenshot screenshot;
    screenshot.Take();
    screenshot.Save();
    setWindowState(Qt::WindowState::WindowActive);
}

void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && shouldMinimizeButton->isChecked()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(areaScreenshot()));
    } else areaScreenshot();
    areaButton->setDown(false);
}

void MainWindow::areaScreenshot() {
    auto* areaScreenshotGrabber = new AreaScreenshotGrabber();
    areaScreenshotGrabber->show();
    setWindowState(Qt::WindowState::WindowActive);
}

void MainWindow::handlePinArea() {
    if(isActiveWindow() && shouldMinimizeButton->isChecked()) {
        setWindowState(Qt::WindowState::WindowMinimized);
        QTimer::singleShot(500, this, SLOT(pinArea()));
    } else pinArea();
    pinButton->setDown(false);
}

void MainWindow::pinArea() {
    auto* pinnedAreaGrabber = new PinnedAreaGrabber();
    pinnedAreaGrabber->show();
    setWindowState(Qt::WindowState::WindowActive);
}
