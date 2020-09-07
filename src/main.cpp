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
    QApplication::setWindowIcon(QIcon("../icons/icon.ico"));

    auto* config = new Config(FileUtils::getApplicationDirectory() + "/config.json");
    config->read();

    if(argc == 2) {
        std::string arg = std::string(argv[1]);
        if(arg == "--full") {
            Screenshot screenshot = {config};
            screenshot.take();
            auto future = screenshot.save();
            if(future != nullptr) future->wait();
            delete config;
            return 0;
        } else if(arg == "--area") {
            auto* areaScreenshotGrabber = new AreaScreenshotGrabber(config);
            areaScreenshotGrabber->setQuitOnClose(config);
            areaScreenshotGrabber->show();
            return QApplication::exec();
        } else {
            printf(
                    "Usage:\n"
                    "    no commands    open gui\n"
                    "    --full         make fullscreen screenshot, upload, then close\n"
                    "    --area         make area screenshot, upload, then close\n"
                    "    --help         show this text\n"
            );
            return 0;
        }
    }

    MainWindow mainWindow(config);
    mainWindow.show();

    return QApplication::exec();
}
