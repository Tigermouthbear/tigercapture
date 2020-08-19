//
// Created by Tigermouthbear on 8/16/20.
//

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QPainter>
#include "Screenshot.h"

#include "FileUtils.h"
#include "../clip/clip.h"

Screenshot::Screenshot(Config *config) {
    this->config = config;
}

void Screenshot::fullscreen() {
    // First pass: Find combined screen size
    int w = 0;
    int h = 0;
    for (auto scr : QGuiApplication::screens()) {
        auto g = scr->geometry();
        w += g.width();
        h = qMax(h, g.height());
    }

    pixmap = {w, h};
    QPainter painter(&pixmap);
    pixmap.fill(Qt::magenta); // if any part is magenta something went wrong!!

    // Second pass, paint onto end screenshot
    int x = 0;
    for (auto scr : QGuiApplication::screens()) {
        auto pix = scr->grabWindow(0);
        painter.drawPixmap(x, 0, pix);
        x += pix.width();
    }
}

void Screenshot::crop(int x, int y, int width, int height) {
    pixmap = pixmap.copy(x, y, width, height);
}

void Screenshot::save() {
    // save file
    std::string loc = FileUtils::genNewImageLocation();
    pixmap.save(QString::fromStdString(loc));

    // upload then copy url to clipboard
    if(config->getUploader() != nullptr) {
        std::string res = config->getUploader()->Upload(loc);
        clip::set_text(res);

        // save entry to log file
        std::ofstream log(FileUtils::getApplicationDirectory() + "/uploads.txt", std::ios_base::app | std::ios_base::out);
        log << loc << "," << res << "\n";
        log.close();
        printf("Saved to %s\n", loc.c_str());
    }
}

QImage Screenshot::image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
