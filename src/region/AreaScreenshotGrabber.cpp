//
// Created by Tigermouthbear on 8/16/20.
//

#include "AreaScreenshotGrabber.h"

#include <iostream>

// fullscreen screenshot right when this is opened
AreaScreenshotGrabber::AreaScreenshotGrabber(Config* config): RegionGrabber() {
    screenshot = new Screenshot(config);
    screenshot->take();
}

// on close, crop and save screenshot
void AreaScreenshotGrabber::closeEvent(QCloseEvent* event) {
    RegionGrabber::closeEvent(event);

    auto selection = getSelection();
    if(hasDragged && !dragging && selection != nullptr) {
        //screenshot->crop(std::min(dragX, mouseX), std::min(dragY, mouseY), std::abs(mouseX - dragX), std::abs(mouseY - dragY));
        screenshot->crop(selection->x(), selection->y(), selection->width(), selection->height());
        screenshot->save();
        delete screenshot;
        screenshot = nullptr;
    }
}

// draw pre cropped screenshot
void AreaScreenshotGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->image());
    RegionGrabber::paintEvent(event);
}
