//
// Created by Tigermouthbear on 8/16/20.
//

#include "Screenshot.h"

#include "FileUtils.h"

void Screenshot::Take() {
    pixmap = QPixmap::grabWindow(0);
}

void Screenshot::Take(int x, int y, int width, int height) {
    pixmap = QPixmap::grabWindow(0, x, y, width, height);
}

void Screenshot::Crop(int x, int y, int width, int height) {
    pixmap = pixmap.copy(x, y, width, height);
}

void Screenshot::Save() {
    pixmap.save(QString::fromStdString(FileUtils::genNewImageLocation()));
}

QImage Screenshot::Image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}