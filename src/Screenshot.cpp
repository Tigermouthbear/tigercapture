//
// Created by Tigermouthbear on 8/16/20.
//

#include "Screenshot.h"

#include <future>
#include <QApplication>
#include <QtGui/QScreen>
#include <QThread>
#include <QtGui/QPainter>
#include <QClipboard>
#include "TigerCapture.hpp"

Screenshot::Screenshot(TigerCapture* tigerCapture) {
    this->tigerCapture = tigerCapture;
}

void Screenshot::take() {
    auto delay = tigerCapture->getConfig()->getDelay();
    if(delay > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    // First pass: Find combined screen size
    QRect total;
    for(auto scr : QGuiApplication::screens()) {
        auto g = scr->geometry();

        int right = g.x() + g.width();
        int bottom = g.y() + g.height();
        total.setWidth(qMax(total.width(), right - total.x()));
        total.setHeight(qMax(total.height(), bottom - total.y()));
        total.setX(qMin(total.x(), g.x()));
        total.setY(qMin(total.y(), g.y()));
    }

    pixmap = {total.width() - total.x(), total.height() - total.y()};
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    // Second pass, paint onto end screenshot
    for(auto scr: QGuiApplication::screens()) {
        auto g = scr->geometry();
        auto pix = scr->grabWindow(0);
        painter.drawPixmap(g.x(), g.y(), g.width(), g.height(), pix);
    }
}

void Screenshot::crop(int x, int y, int width, int height) {
    pixmap = pixmap.copy(x, y, width, height);
}

// save image and pass to uploader
std::future<void>* Screenshot::save() {
    auto* out = new std::future<void>;
    *out = std::async([=]() {
        // clear so user doesnt accidentally paste something else while waiting for the image to upload
        TC::Clipboard::clearClipboard();

        // save screenshot to folder
        std::string loc = TC::Files::genNewImageLocation();
        pixmap.save(QString::fromStdString(loc));

        // upload then copy url to clipboard
        if(tigerCapture->getConfig()->getUploader() != nullptr && tigerCapture->getConfig()->getUploader()->check(Uploader::IMAGE_UPLOADER)) {
            // upload, if response is empty break
            std::string res = tigerCapture->getConfig()->getUploader()->Upload(loc);
            if(res.empty()) return;

            // copy response
            TC::Clipboard::copyToClipboard(res);

            // display notification
            tigerCapture->getSystemTray()->showMessage("TigerCapture", ("Uploaded to: " + res).c_str());
        } else {
            // save location to log file
            std::ofstream log(TC::Files::getUploadsLogFile(), std::ios_base::app | std::ios_base::out);
            log << loc << ",\n";
            log.close();
            printf("Saved to: %s\n", loc.c_str());
        }

        // update explorer
        tigerCapture->updateUploadsExplorer();

        delete this;
    });
    return out;
}

QImage Screenshot::image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
