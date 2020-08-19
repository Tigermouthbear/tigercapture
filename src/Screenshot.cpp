//
// Created by Tigermouthbear on 8/16/20.
//

#include "Screenshot.h"

#include "FileUtils.h"
#include "../clip/clip.h"

Screenshot::Screenshot(Config *config) {
    this->config = config;
}

void Screenshot::take() {
    pixmap = QPixmap::grabWindow(0);
}

void Screenshot::take(int x, int y, int width, int height) {
    pixmap = QPixmap::grabWindow(0, x, y, width, height);
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
    }
}

QImage Screenshot::image() {
    return pixmap.toImage();
}

Screenshot::~Screenshot() {
    pixmap.detach();
}
