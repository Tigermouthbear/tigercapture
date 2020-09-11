//
// Created by Tigermouthbear on 9/11/20.
//

#include "SystemTray.h"

#include <QApplication>

SystemTray::SystemTray(const std::string& icon) {
    setIcon(QIcon(icon.c_str()));

    // add menu
    menu = new QMenu();
    exitAction = menu->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));
    setContextMenu(menu);
}

void SystemTray::exit() {
    QApplication::exit();
}
