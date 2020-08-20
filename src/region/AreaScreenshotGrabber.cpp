//
// Created by Tigermouthbear on 8/16/20.
//

#include "AreaScreenshotGrabber.h"

#include <iostream>

// fullscreen screenshot right when this is opened
AreaScreenshotGrabber::AreaScreenshotGrabber(Config* config, QSystemTrayIcon* iconIn): RegionGrabber() {
    screenshot = new Screenshot(config);
    screenshot->take();
    icon = iconIn;
}

// on close, crop and save screenshot
void AreaScreenshotGrabber::closeEvent(QCloseEvent* event) {
    if(hasDragged && !dragging) {
        screenshot->crop(std::min(dragX, mouseX), std::min(dragY, mouseY), std::abs(mouseX - dragX), std::abs(mouseY - dragY));
        screenshot->save(icon);
        delete screenshot;
        screenshot = nullptr;
        RegionGrabber::closeEvent(event);
    }
}

// draw pre cropped screenshot
void AreaScreenshotGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->image());
    RegionGrabber::paintEvent(event);
}

std::future<void>* AreaScreenshotGrabber::getCallback() {
    return &callback;
}
