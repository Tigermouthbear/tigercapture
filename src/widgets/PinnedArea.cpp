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
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    // set size and position
    resize(this->image.width(), this->image.height());
    move(x, y);
}

// start drag on click
void PinnedArea::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragX = event->globalX() - x;
        dragY = event->globalY() - y;
        dragging = true;
    } else if(event->button() == Qt::RightButton) close();
}

// move widget with mouse while dragging
void PinnedArea::mouseMoveEvent(QMouseEvent* event) {
    if(dragging) {
        x = event->globalX() - dragX;
        y = event->globalY() - dragY;
        move(x, y);
    }
}

void PinnedArea::mouseReleaseEvent(QMouseEvent* event) {
    if (dragging) {
        // If this widget is out of all screen bounds then close
        bool visible = false;
        int right = x + width();
        int bottom = y + height();
        for (auto scr : QGuiApplication::screens()) {
            auto g = scr->geometry();
            if (!(x < g.right() && right > g.left() && y > g.bottom() && bottom < g.top())) {
                visible = true;
                break;
            }
        }
        if (!visible) close();
        if(event->button() == Qt::LeftButton) dragging = false;
    }
}

void PinnedArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, image, 0, 0, -1, -1, Qt::AutoColor);
    Utils::drawOutlineBox(&painter, 0, 0, image.width() - 1, image.height() - 1);
}