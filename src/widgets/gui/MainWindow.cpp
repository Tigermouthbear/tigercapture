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
    connect(dragUploadButton, SIGNAL(released()), this, SLOT(handleDragUpload()));

    configButton = new QPushButton("Config", this);
    layout->addWidget(configButton, 4, 0);
    connect(configButton, SIGNAL(released()), this, SLOT(handleConfig()));

    layout->setColumnMinimumWidth(1, 300);
    uploadsExplorerWidget = new UploadsExplorerWidget(this);
    layout->addWidget(uploadsExplorerWidget, 0, 1, 5, 1);

    // set size and position from config
    move(tigerCapture->getConfig()->getX() - x(), tigerCapture->getConfig()->getY() - y());

    // set column sizes and stretch
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);

    QObject::connect(this, SIGNAL(updateExplorerSignal()), this, SLOT(updateExplorerSlot()), Qt::QueuedConnection);
}

void MainWindow::updateExplorer() {
    emit updateExplorerSignal();
}

void MainWindow::updateExplorerSlot() {
    uploadsExplorerWidget->updateUploads();
}

void MainWindow::activateWindow() {
    setWindowState(Qt::WindowState::WindowActive);
    show();
}

// minimize, if needed, delay then actually fullscreen screenshot
void MainWindow::handleFullScreenshot() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, tigerCapture, SLOT(fullScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else tigerCapture->fullScreenshot();
}

// minimize, if needed, delay then actually open area screenshot
void MainWindow::handleAreaScreenshot() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, tigerCapture, SLOT(areaScreenshot()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else tigerCapture->areaScreenshot();
}

// minimize, if needed, delay then actually open pin region grabber
void MainWindow::handlePinArea() {
    if(isActiveWindow() && tigerCapture->getConfig()->shouldMinimize()) {
        hide();
        QTimer::singleShot(500, tigerCapture, SLOT(pinArea()));
        QTimer::singleShot(501, this, SLOT(activateWindow()));
    } else tigerCapture->pinArea();
}

void MainWindow::handleDragUpload() {
    tigerCapture->dragUpload();
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
    tigerCapture->setWindowClosed();
}
