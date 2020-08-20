//
// Created by Tigermouthbear on 8/16/20.
//

#include "RegionGrabber.h"

#include <QtGui/QGuiApplication>
#include <QScreen>
#include <QPainter>
#include "../Utils.h"
#include <QApplication>

RegionGrabber::RegionGrabber(): QWidget(nullptr, Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool) {
    // set transparent
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    // set fullscreen
    QRect total;
    for (auto scr : QGuiApplication::screens()) {
        auto g = scr->geometry();

        int right = g.x() + g.width();
        int bottom = g.y() + g.height();
        total.setWidth(qMax(total.width(), right - total.x()));
        total.setHeight(qMax(total.height(), bottom - total.y()));
        total.setX(qMin(total.x(), g.x()));
        total.setY(qMin(total.y(), g.y()));
    }
    resize(total.width() - total.x(), total.height() - total.y());
    move(total.x() - x(), total.y() - y());

    // grab inputs
    setMouseTracking(true);
    grabKeyboard();
    grabMouse();
}

void RegionGrabber::closeEvent(QCloseEvent* event) {
    releaseKeyboard();
    releaseMouse();

    if(quit) QApplication::quit();
}

void RegionGrabber::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Escape) close();
}

void RegionGrabber::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragX = event->x();
        dragY = event->y();
        dragging = true;
        hasDragged = true;
    }
    update();
}

void RegionGrabber::mouseMoveEvent(QMouseEvent* event) {
    mouseX = event->x();
    mouseY = event->y();
    update();
}

void RegionGrabber::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragging = false;
        close();
    }
    update();
}

void RegionGrabber::paintEvent(QPaintEvent* event) {
    // draw semi-transparent background
    QColor backgroundColor = palette().light().color();
    backgroundColor.setAlpha(100);
    QPainter painter(this);
    painter.fillRect(rect(), backgroundColor);

    if(dragging) {
        Utils::drawOutlineBox(&painter, dragX, dragY, mouseX - dragX, mouseY - dragY);
    }
}

void RegionGrabber::quitOnClose(bool value) {
    quit = value;
}
