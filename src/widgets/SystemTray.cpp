//
// Created by Tigermouthbear on 9/11/20.
//

#include "SystemTray.h"

#include <QApplication>

SystemTray::SystemTray(TigerCapture* tigerCapture, const std::string& icon): QSystemTrayIcon() {
    this->tigerCapture = tigerCapture;
    setIcon(QIcon(icon.c_str()));

    // add menu
    menu = new QMenu();
    exitAction = menu->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));
    setContextMenu(menu);

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(openWindow()));
}

void SystemTray::openWindow() {
    tigerCapture->openWindow();
}

void SystemTray::exit() {
    QApplication::exit();
}
