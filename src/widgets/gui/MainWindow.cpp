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
    setMinimumSize(410, 205);

    // create layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QGridLayout(centralWidget);

    // initialize buttons
    fullButton = new QPushButton("Full Screenshot", this);
    connect(fullButton, SIGNAL(released()), this, SLOT(handleFullScreenshot()));

    areaButton = new QPushButton("Area Screenshot", this);
    connect(areaButton, SIGNAL(released()), this, SLOT(handleAreaScreenshot()));

    pinButton = new QPushButton("Pin Area", this);
    connect(pinButton, SIGNAL(released()), this, SLOT(handlePinArea()));

    dragUploadButton = new QPushButton("Drag and Drop");
    connect(dragUploadButton, SIGNAL(released()), this, SLOT(handleDragUpload()));

    configButton = new QPushButton("Config", this);
    connect(configButton, SIGNAL(released()), this, SLOT(handleConfig()));

    uploadsExplorerWidget = new UploadsExplorerWidget(this);

    // set size and position from config
    move(tigerCapture->getConfig()->getX() - x(), tigerCapture->getConfig()->getY() - y());
    resize(tigerCapture->getConfig()->getWidth(), tigerCapture->getConfig()->getHeight());

    // add widgets in correct mode
    addWidgets(isCompact(size()));

    QObject::connect(this, SIGNAL(updateExplorerSignal()), this, SLOT(updateExplorerSlot()), Qt::QueuedConnection);
}

void MainWindow::addWidgets(bool compact, bool refresh) {
    if(refresh) {
        layout->removeWidget(fullButton);
        layout->removeWidget(areaButton);
        layout->removeWidget(pinButton);
        layout->removeWidget(dragUploadButton);
        layout->removeWidget(configButton);
        layout->removeWidget(uploadsExplorerWidget);
    }

    if(compact) {
        layout->addWidget(fullButton, 0, 0);
        layout->addWidget(areaButton, 1, 0);
        layout->addWidget(pinButton, 2, 0);
        layout->addWidget(dragUploadButton, 3, 0);
        layout->addWidget(configButton, 4, 0);
        layout->addWidget(uploadsExplorerWidget, 0, 1, 5, 1);
    } else {
        layout->addWidget(fullButton, 0, 0);
        layout->addWidget(areaButton, 0, 1);
        layout->addWidget(pinButton, 0, 2);
        layout->addWidget(dragUploadButton, 0, 3);
        layout->addWidget(configButton, 0, 4);
        layout->addWidget(uploadsExplorerWidget, 1, 0, 1, 5);
    }

    uploadsExplorerWidget->setCompact(compact);
}

bool MainWindow::isCompact(QSize size) {
    return size.width() <= 530 && size.height() <= 230;
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
    configWidget->move(x(), y() + QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));
    configWidget->exec();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    bool compact = isCompact(event->size());
    if(isCompact(event->oldSize()) != compact) addWidgets(compact, true);
    QWidget::resizeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    tigerCapture->getConfig()->setX(x());
    tigerCapture->getConfig()->setY(y());
    tigerCapture->getConfig()->setWidth(width());
    tigerCapture->getConfig()->setHeight(height());
    tigerCapture->getConfig()->write();
    tigerCapture->setWindowClosed();
}
