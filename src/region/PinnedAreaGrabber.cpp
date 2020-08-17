//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedAreaGrabber.h"

#include "PinnedArea.h"

PinnedAreaGrabber::PinnedAreaGrabber(): RegionGrabber() {
    screenshot = new Screenshot();
    screenshot->Take();
}

// crop then pin image to screen on close
void PinnedAreaGrabber::closeEvent(QCloseEvent* event) {
    RegionGrabber::closeEvent(event);

    if(hasDragged && !dragging) {
        screenshot->Crop(std::min(dragX, mouseX), std::min(dragY, mouseY), std::abs(mouseX - dragX), std::abs(mouseY - dragY));

        auto* pinnedArea = new PinnedArea(screenshot->Image());
        pinnedArea->show();
    }
}

// draw pre cropped screenshot
void PinnedAreaGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->Image());
    RegionGrabber::paintEvent(event);
}
