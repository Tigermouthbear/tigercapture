//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedAreaGrabber.h"

#include "../widgets/PinnedArea.h"

PinnedAreaGrabber::PinnedAreaGrabber(TigerCapture* tigerCapture): RegionGrabber() {
    screenshot = new Screenshot(tigerCapture);
    screenshot->take();
}

// draw pre cropped screenshot
void PinnedAreaGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->image());
    RegionGrabber::paintEvent(event);
}

void PinnedAreaGrabber::onFinish() {
    releaseKeyboard();
    releaseMouse();
    hide();

    auto selection = getSelection();
    if(hasDragged && !dragging && selection != nullptr) {
        screenshot->crop(selection->x(), selection->y(), selection->width(), selection->height());
        auto* pinnedArea = new PinnedArea(selection->x(), selection->y(), screenshot->image());
        pinnedArea->show();
        delete screenshot;
    }

    close();
}
