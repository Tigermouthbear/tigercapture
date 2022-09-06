//
// Created by Tigermouthbear on 10/26/20.
//

#include <QtWidgets/QApplication>
#include <signal.h>

#include "TigerCapture.h"
#include "IpcServer.h"
#include "IpcClient.h"

static void signal_handler(int signo) {
    QApplication::quit();
}

int main(int argc, char* argv[]) { 
    // parse command line option
    qint8 mode;
    QString str("");
    if(argc >= 2) {
        std::string arg = std::string(argv[1]);
        if(arg == "--headless" || arg == "-h") mode = 0;
        else if(arg == "--stop" || arg == "-s") mode = 2;
        else if(arg == "--full" || arg == "-f") mode = 3;
        else if(arg == "--area" || arg == "-a") mode = 4;
        else if(arg == "--pin" || arg == "-p") mode = 5;
        else if(arg == "--upload" || arg == "-u") {
            if(argc == 3) {
                QFileInfo fi = QFileInfo(QString(argv[2]));
                if(!fi.exists()) {
                    std::cout << "File '" << argv[2] << "' does not exist!\n";
                    return 0;
                }
                str = fi.absoluteFilePath();
            }
            mode = 6;
        } else {
            // TODO: FIX THIS
            std::cout <<
            "Usage: tigercapture [OPTION]\n"
            "If no other tigercapture process is running, a new instance will be started and take over this process.\n\n"
            "Options:\n"
            "    no option              start tigercapture or open window\n"
            "    -h, --headless         start tigercapture without opening window\n"
            "    -s, --stop             stop tigercapture\n"
            "    -f, --full             take fullscreen screenshot\n"
            "    -a, --area             take area screenshot\n"
            "    -p, --pin              pin screenshot of area to display\n"
            "    -u, --upload <file?>   upload file or open drag and drop upload window if no file given\n"
            "    --help                 show this text\n";
            return 0;
        }
    } else mode = 1;

    // try to connect to running instance and signal option
    IpcClient client([](char* str, quint16 len) {
        std::cout << str;
    });
    if(client.isAlive()) {
        client.sendSignal(mode, (char*) str.toStdString().c_str());
        client.waitForRead();
        return 0;
    }

    // if signal is to stop, just don't even start new instance
    if(mode == 2) return 0;

    // otherwise, start a new instance
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // handle interuppt so that IpcServer can be closed nicely
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // create instance and IpcServer that handles incoming signals
    TigerCapture* tigerCapture = new TigerCapture();
    IpcServer server(tigerCapture, [=](qint8 sig, char* str) {
        // client has to be flushed for commands that don't already flush on success
        if(sig == 1) {
            tigerCapture->openWindow();
            tigerCapture->getOutput()->flush();
        } else if(sig == 2) {
            tigerCapture->getOutput()->flush();
            QApplication::quit();
        } else if(sig == 3) tigerCapture->fullScreenshot();
        else if(sig == 4) tigerCapture->areaScreenshot();
        else if(sig == 5) {
            tigerCapture->pinArea();
            tigerCapture->getOutput()->flush();
        } else if(sig == 6) {
            if(*str == 0) {
                tigerCapture->dragUpload();
                tigerCapture->getOutput()->flush();
            } else tigerCapture->upload(std::string(str));
        }
    });
    if(!server.isAlive()) { // just shutdown on fail
        delete tigerCapture;
        return 1;
    }

    // set window icon
    QApplication::setWindowIcon(QIcon(tigerCapture->getResource("icons/icon.ico").c_str()));

    // signal option to the server we just created
    if(mode != 0) server.handleSignal(mode, (char*) str.toStdString().c_str());

    // main loop and cleanup
    int r = QApplication::exec();
    delete tigerCapture;
    return r;
}
