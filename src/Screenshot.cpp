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
#include "FileUtils.h"
#include "Clipboard.h"

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
    for(auto scr : QGuiApplication::screens()) {
        auto g = scr->geometry();
        auto pix = scr->grabWindow(0);
        painter.drawPixmap(g.x(), g.y(), g.width(), g.height(), pix);
    }
}

void Screenshot::crop(int x, int y, int width, int height) {
    pixmap = pixmap.copy(x, y, width, height);
}

static void screenshotCallback(void* tigerCapture, const std::string& res) {
    if(res.empty()) return;

    // copy response
    Clipboard::copyToClipboard(res);

    // display notification
    ((TigerCapture*) tigerCapture)->getSystemTray()->showMessage("TigerCapture", ("Uploaded to: " + res).c_str());
}

std::future<void>* Screenshot::save() {
    // TODO: Broken
    //if (tigerCapture->shouldClipboard()) {
    //    Clipboard::copyToClipboard(pixmap);
    //} else {
    Clipboard::clearClipboard(); // clear so user doesnt accidentally paste something else while waiting for the image to upload
    //}

    // save file
    std::string loc = FileUtils::genNewImageLocation();
    pixmap.save(QString::fromStdString(loc));

    // upload then copy url to clipboard
    if(tigerCapture->getConfig()->getUploader() != nullptr) {
        // upload async, if response is empty break
        return tigerCapture->getConfig()->getUploader()->Upload(loc, tigerCapture, screenshotCallback);
    }
    return nullptr;
}

QImage Screenshot::image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
