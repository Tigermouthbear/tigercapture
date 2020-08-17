//
// Created by Tigermouthbear on 8/16/20.
//

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.Create();

    return QApplication::exec();
}