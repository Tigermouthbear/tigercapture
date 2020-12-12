//
// Created by Tigermouthbear on 10/26/20.
//

#include <QtWidgets/QApplication>

#include "TigerCapture.h"
#include "Singleton.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    auto* tigerCapture = new TigerCapture();

    if(argc == 2) {
        // make sure only one command interface is running
        Singleton singleton(7197);
        if(!singleton()) {
            printf("Tigercapture already running in background! Exiting...\n");
            return 0;
        }

        std::string arg = std::string(argv[1]);
        if(arg == "--full") {
            Screenshot screenshot = {tigerCapture};
            screenshot.take();
            auto future = screenshot.save();
            if(future != nullptr) future->wait();
            delete tigerCapture;
            return 0;
        } else if(arg == "--area") {
            auto* areaScreenshotGrabber = (new AreaScreenshotGrabber(tigerCapture));
            areaScreenshotGrabber->show();
            areaScreenshotGrabber->setQuitOnClose();
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

    // make sure two instances aren't running at once
    Singleton singleton(7196);
    if(!singleton()) {
        printf("Tigercapture already running! Exiting...\n");
        return 0;
    }

    // set window icon then open window
    QApplication::setWindowIcon(QIcon(tigerCapture->getResource("icons/icon.ico").c_str()));
    tigerCapture->openWindow();

    return QApplication::exec();
}
