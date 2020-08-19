//
// Created by Tigermouthbear on 8/16/20.
//

#include "Screenshot.h"

#include "FileUtils.h"
#include "../clip/clip.h"

Screenshot::Screenshot(Config *config) {
    this->config = config;
}

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
    // save file
    std::string loc = FileUtils::genNewImageLocation();
    pixmap.save(QString::fromStdString(loc));

    // upload then copy url to clipboard
    if(config->shouldUpload()) {
        std::string res = config->getUploader()->Upload(loc);
        clip::set_text(res);
    }
}

QImage Screenshot::Image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
