//
// Created by Tigermouthbear on 8/16/20.
//

#include <QApplication>
#include <iostream>
#include "widgets/MainWindow.h"
#include "FileUtils.h"
#include "TigerCapture.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    auto* tigerCapture = new TigerCapture(FileUtils::getApplicationDirectory() + "/config.json");
    tigerCapture->read();

    QApplication::setWindowIcon(QIcon(tigerCapture->getResource("icons/icon.ico").c_str()));

    if(argc == 2) {
        std::string arg = std::string(argv[1]);
        if(arg == "--full") {
            Screenshot screenshot = {tigerCapture};
            screenshot.take();
            auto future = screenshot.save();
            if(future != nullptr) future->wait();
            delete tigerCapture;
            return 0;
        } else if(arg == "--area") {
            auto* areaScreenshotGrabber = new AreaScreenshotGrabber(tigerCapture);
            areaScreenshotGrabber->setQuitOnClose(tigerCapture);
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

    MainWindow mainWindow(tigerCapture);
    mainWindow.show();

    return QApplication::exec();
}
