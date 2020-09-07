//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedAreaGrabber.h"

#include "../widgets/PinnedAreaWindow.h"

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

        // This is necessary so it dosent go in the taskbar
        auto* window = new QMainWindow();
        window->setWindowFlag(Qt::FramelessWindowHint);
        window->setWindowFlag(Qt::WindowStaysOnTopHint);
        window->setWindowFlag(Qt::Tool);
        window->resize(1, 1); // No one will notice this pixel... I hope
        window->show();

        auto* pinnedArea = new PinnedAreaWindow(selection->x(), selection->y(), screenshot->image(), window);
        pinnedArea->show();
    }

    close();
}

PinnedAreaGrabber::~PinnedAreaGrabber() {
    delete screenshot;
}
