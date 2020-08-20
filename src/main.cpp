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

    QSystemTrayIcon icon;
    // icon->setIcon(QIcon{}); TODO: icon
    icon.show();
    auto* config = new Config(FileUtils::getApplicationDirectory() + "/config.json");
    config->read();

    if(argc == 2) {
        std::string arg = std::string(argv[1]);
        if(arg == "--full") {
            MainWindow::fullScreenshot(&icon, config);
            return 0;
        } else if(arg == "--area") {
            MainWindow::areaScreenshot(&icon, config)->quitOnClose(true);
            return QApplication::exec();
        } else {
            printf(
                    "Usage:\n"
                    "    no commands    open gui\n"
                    "    --full         make fullscreen screenshot, upload, then close\n"
                    "    --area         make area screenshot, upload, then close\n"
                    "    --help         show this text"
                );
            return 0;
        }
    }

    MainWindow mainWindow(&icon, config);
    mainWindow.show();

    int out = QApplication::exec();

    config->setX(mainWindow.x());
    config->setY(mainWindow.y());
    config->write();

    return out;
}
