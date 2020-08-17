//
// Created by Tigermouthbear on 8/16/20.
//

#include "AreaScreenshotGrabber.h"
#include <iostream>

// take screenshot right when this is opened
AreaScreenshotGrabber::AreaScreenshotGrabber(): RegionGrabber() {
    screenshot = new Screenshot();
    screenshot->Take();
}

// on close, crop and save screenshot
void AreaScreenshotGrabber::closeEvent(QCloseEvent* event) {
    RegionGrabber::closeEvent(event);

    if(hasDragged && !dragging) {
        screenshot->Crop(std::min(dragX, mouseX), std::min(dragY, mouseY), std::abs(mouseX - dragX), std::abs(mouseY - dragY));
        screenshot->Save();
    }
}

// draw pre cropped screenshot
void AreaScreenshotGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->Image());
    RegionGrabber::paintEvent(event);
}
