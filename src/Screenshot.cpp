//
// Created by Tigermouthbear on 8/16/20.
//

#include "Screenshot.h"

#include <future>
#include <QApplication>
#include <QtGui/QScreen>
#include <QtGui/QPainter>
#include <QClipboard>
#include <QSystemTrayIcon>
#include "FileUtils.h"

Screenshot::Screenshot(Config *config) {
    this->config = config;
}

void Screenshot::take() {
    auto delay = config->getDelay();
    if (delay > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    // First pass: Find combined screen size
    QRect total;
    for (auto scr : QGuiApplication::screens()) {
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
    for (auto scr : QGuiApplication::screens()) {
        auto g = scr->geometry();
        auto pix = scr->grabWindow(0);
        painter.drawPixmap(g.x(), g.y(), g.width(), g.height(), pix);
    }
}

void Screenshot::crop(int x, int y, int width, int height) {
    pixmap = pixmap.copy(x, y, width, height);
}

/// make sure parameter is valid after screenshot is uploaded, i.e dont create a future from a stack allocated lambda
std::future<void> Screenshot::save(std::function<void()> callback) {
    return std::async([&] {
        this->save();
        if(callback) callback();
    });
}

void Screenshot::save() {
    // save file
    std::string loc = FileUtils::genNewImageLocation();
    pixmap.save(QString::fromStdString(loc));

    // upload then copy url to clipboard
    if(config->getUploader() != nullptr) {
        // upload, if response is empty break
        std::string res = config->getUploader()->Upload(loc);
        if(res.empty()) return;

        // copy response
        auto clip = QApplication::clipboard();
        clip->setText(res.c_str());

        // display notification
        config->getSystemTrayIcon()->showMessage("TigerCapture", ("Uploaded to: " + res).c_str());

        // save entry to log file
        std::ofstream log(FileUtils::getApplicationDirectory() + "/uploads.txt", std::ios_base::app | std::ios_base::out);
        log << loc << "," << res << "\n";
        log.close();
        printf("Saved to: %s\n", loc.c_str());
    }
}

QImage Screenshot::image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
