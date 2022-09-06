//
// Created by Tigermouthbear on 8/16/20.
//

#include "Screenshot.h"

#include <future>
#include <QApplication>
#include <QtGui/QScreen>
#include <QThread>
#include <QtGui/QPainter>
#include <thread>
#include "TigerCapture.hpp"

#include <QRect>
#include <iostream>

Screenshot::Screenshot(TigerCapture* tigerCapture) {
    this->tigerCapture = tigerCapture;
}

void Screenshot::take() {
    auto delay = tigerCapture->getConfig()->getDelay();
    if(delay > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    auto screens = QGuiApplication::screens();

    // First pass: Find combined screen size and highest devicePixelRatio
    qreal dpr = 1;
    QRect total;
    for(auto scr: screens) {
        auto g = scr->geometry();
        int right = g.x() + g.width();
        int bottom = g.y() + g.height();

        total.setWidth(qMax(total.width(), right - total.x()));
        total.setHeight(qMax(total.height(), bottom - total.y()));
        total.setX(qMin(total.x(), g.x()));
        total.setY(qMin(total.y(), g.y()));

        qreal cdpr = scr->devicePixelRatio();
        if(cdpr > dpr) dpr = cdpr;
    }

    pixmap = { (int)((total.width() - total.x()) * dpr), (int)((total.height() - total.y()) * dpr) };
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    // Second pass, paint onto end screenshot
    for(auto scr = screens.crbegin(); scr != screens.crend(); ++scr) {
        auto g = (*scr)->geometry();
        auto pix = (*scr)->grabWindow(0);
        painter.drawPixmap(g.x() - total.x(), g.y() - total.y(), g.width(), g.height(), pix);
    }
}

void Screenshot::crop(int x, int y, int width, int height) {
    pixmap = pixmap.copy(x, y, width, height);
}

// save image and pass to uploader
std::future<void> Screenshot::save() {
    return std::async([=]() {
        // clear so user doesnt accidentally paste something else while waiting for the image to upload
        TC::Clipboard::clearClipboard();

        // copy to clipboard if wanted
        if(tigerCapture->getConfig()->shouldClipboard()) {
            TC::Clipboard::copyToClipboard(pixmap);
        }

        // save screenshot to folder
        std::string loc = TC::Files::genNewImageLocation();
        pixmap.save(QString::fromStdString(loc));

        // upload then copy url to clipboard
        if(tigerCapture->getConfig()->getUploader() != nullptr && tigerCapture->getConfig()->getUploader()->check(Uploader::IMAGE_UPLOADER)) {
            // upload, if response is empty break
            std::string res = tigerCapture->getConfig()->getUploader()->upload(loc);
            if(res.empty()) return;

            // copy response
            TC::Clipboard::copyToClipboard(res);
        } else {
            // save location to log file
            std::ofstream log(TC::Files::getUploadsLogFile(), std::ios_base::app | std::ios_base::out);
            log << loc << ",\n";
            log.close();
            tigerCapture->getOutput()->write("Saved to: " + loc + "\n");
            tigerCapture->getOutput()->flush();

            // update screenshot explorer
            tigerCapture->getMainWindow()->updateExplorer();
        }

        delete this;
    });
}

QImage Screenshot::image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
