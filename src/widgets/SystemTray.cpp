//
// Created by Tigermouthbear on 9/11/20.
//

#include "SystemTray.h"

#include <QApplication>
#include <QTimer>

SystemTray::SystemTray(TigerCapture* tigerCapture, const std::string& icon): QSystemTrayIcon() {
    this->tigerCapture = tigerCapture;
    setIcon(QIcon(icon.c_str()));

    // add menu
    menu = new QMenu();

    // add actions
    fullAction = menu->addAction("Full Screenshot");
    connect(fullAction, SIGNAL(triggered()), this, SLOT(fullScreenshot()));

    areaAction = menu->addAction("Area Screenshot");
    connect(areaAction, SIGNAL(triggered()), this, SLOT(areaScreenshot()));

    pinAction = menu->addAction("Pin Area");
    connect(pinAction, SIGNAL(triggered()), this, SLOT(pinArea()));

    exitAction = menu->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    setContextMenu(menu);

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(openWindow()));
}

void SystemTray::openWindow() {
    tigerCapture->openWindow();
}

void SystemTray::fullScreenshot() {
    QTimer::singleShot(500, tigerCapture, SLOT(fullScreenshot()));
}

void SystemTray::areaScreenshot() {
    QTimer::singleShot(500, tigerCapture, SLOT(areaScreenshot()));
}

void SystemTray::pinArea() {
    QTimer::singleShot(500, tigerCapture, SLOT(pinArea()));
}

void SystemTray::exit() {
    QApplication::exit();
}
