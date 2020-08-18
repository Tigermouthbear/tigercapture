//
// Created by Tigermouthbear on 8/16/20.
//

#include <QApplication>
#include <iostream>
#include "widgets/MainWindow.h"
#include "FileUtils.h"
#include "Config.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    auto* config = new Config(FileUtils::getApplicationDirectory() + "/config.json");
    config->Read();

    if(argc == 2) {
        std::string arg = std::string(argv[1]);
        if(arg == "full") {
            MainWindow::fullScreenshot();
            return 0;
        } else if(arg == "area") {
            MainWindow::areaScreenshot()->quitOnClose(true);
        } else {
            std::cout << "Argument not found!" << std::endl;
            return 1;
        }
    }

    MainWindow mainWindow(config);
    mainWindow.show();

    return QApplication::exec();
}
