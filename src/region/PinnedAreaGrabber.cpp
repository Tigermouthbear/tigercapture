//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedAreaGrabber.h"

#include "../widgets/PinnedArea.h"

PinnedAreaGrabber::PinnedAreaGrabber(TigerCapture* tigerCapture): RegionGrabber() {
    this->tigerCapture = tigerCapture;
    screenshot = new Screenshot(tigerCapture);
    screenshot->take();
}

void PinnedAreaGrabber::open() {
    tigerCapture->setTakingScreenshot(true);
    show();
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
    if(hasDragged && selection != nullptr) {
        screenshot->crop(selection->x(), selection->y(), selection->width(), selection->height());
        (new PinnedArea(selection->x(), selection->y(), screenshot->image()))->show();
        delete screenshot;
    }

    close();
    tigerCapture->setTakingScreenshot(false);
}
