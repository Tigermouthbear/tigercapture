//
// Created by Tigermouthbear on 8/16/20.
//

#include "PinnedArea.h"

#include <QtGui/QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <utility>
#include <QMouseEvent>
#include "../Utils.h"

PinnedArea::PinnedArea(int x, int y, QImage image): QWidget(nullptr, Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool) {
    this->x = x;
    this->y = y;
    this->image = std::move(image);

    // set transparent
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    // set size and position
    resize(this->image.width(), this->image.height());
    move(x, y);

    // set window size for position clamping
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    screenWidth = screenGeometry.width();
    screenHeight = screenGeometry.height();
}

// start drag on click
void PinnedArea::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragX = event->globalX() - x;
        dragY = event->globalY() - y;
        dragging = true;
    } else if(event->button() == Qt::RightButton) close();
}

// move widget with mouse while dragging (also clamp to keep it on screen)
void PinnedArea::mouseMoveEvent(QMouseEvent* event) {
    if(dragging) {
        x = Utils::clamp(event->globalX() - dragX, 0, screenWidth - image.width());
        y = Utils::clamp(event->globalY() - dragY, 0, screenHeight - image.height());
        move(x, y);
    }
}

void PinnedArea::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) dragging = false;
}

void PinnedArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
    Utils::drawOutlineBox(&painter, 0, 0, image.width() - 1, image.height() - 1);
}
