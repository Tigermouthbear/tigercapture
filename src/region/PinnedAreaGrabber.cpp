//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedAreaGrabber.h"

#include "../widgets/PinnedArea.h"

PinnedAreaGrabber::PinnedAreaGrabber(Config* config): RegionGrabber() {
    screenshot = new Screenshot(config);
    screenshot->fullscreen();
}

// crop then pin image to screen on close
void PinnedAreaGrabber::closeEvent(QCloseEvent* event) {
    RegionGrabber::closeEvent(event);

    if(hasDragged && !dragging) {
        int x = std::min(dragX, mouseX);
        int y = std::min(dragY, mouseY);
        screenshot->crop(x, y, std::abs(mouseX - dragX), std::abs(mouseY - dragY));

        auto* pinnedArea = new PinnedArea(x, y, screenshot->image());
        pinnedArea->show();

        delete screenshot;
    }
}

// draw pre cropped screenshot
void PinnedAreaGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->image());
    RegionGrabber::paintEvent(event);
}
