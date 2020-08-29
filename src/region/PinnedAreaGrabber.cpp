//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedAreaGrabber.h"

#include "../widgets/PinnedAreaWindow.h"

PinnedAreaGrabber::PinnedAreaGrabber(Config* config): RegionGrabber() {
    screenshot = new Screenshot(config);
    screenshot->take();
}

// crop then pin image to screen on close
void PinnedAreaGrabber::closeEvent(QCloseEvent* event) {
    RegionGrabber::closeEvent(event);

    auto selection = getSelection();
    if(hasDragged && !dragging && selection != nullptr) {
        screenshot->crop(selection->x(), selection->y(), selection->width(), selection->height());

        auto* pinnedArea = new PinnedAreaWindow(selection->x(), selection->y(), screenshot->image());
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
