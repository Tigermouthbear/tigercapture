//
// Created by Tigermouthbear on 8/16/20.
//

#include "AreaScreenshotGrabber.h"

#include <QApplication>
#include <QThread>
#include <future>

// fullscreen screenshot right when this is opened
AreaScreenshotGrabber::AreaScreenshotGrabber(TigerCapture* tigerCapture): RegionGrabber() {
    this->tigerCapture = tigerCapture;
    screenshot = new Screenshot(tigerCapture);
    screenshot->take();
}

void AreaScreenshotGrabber::open() {
    tigerCapture->setTakingScreenshot(true);
    show();
}

// draw pre cropped screenshot
void AreaScreenshotGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, screenshot->image());
    RegionGrabber::paintEvent(event);
}

// on close, crop and save screenshot
void AreaScreenshotGrabber::onFinish() {
    releaseKeyboard();
    releaseMouse();
    hide();

    auto selection = getSelection();
    if(hasDragged && selection != nullptr) {
        screenshot->crop(selection->x(), selection->y(), selection->width(), selection->height());
        screenshot->save();
    }
    
    close();
    tigerCapture->setTakingScreenshot(false);
}
