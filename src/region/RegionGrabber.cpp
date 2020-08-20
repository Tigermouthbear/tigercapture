//
// Created by Tigermouthbear on 8/16/20.
//

#include "RegionGrabber.h"

#include <QtGui/QGuiApplication>
#include <QScreen>
#include <QPainter>
#include "../Utils.h"
#include <QApplication>
#include <QtGui/QPainterPath>

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
        setSelection(new QRect(dragX, dragY, 0, 0));
    }
    update();
}

void RegionGrabber::mouseMoveEvent(QMouseEvent* event) {
    setSelection(new QRect(dragX, dragY, event->x() - dragX, event->y() - dragY));
    update();
}

void RegionGrabber::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragging = false;
        setSelection(new QRect(dragX, dragY, event->x() - dragX, event->y() - dragY));
        close();
    }
    update();
}

void RegionGrabber::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QRect total = rect();

    // draw semi-transparent background
    QColor backgroundColor = palette().light().color();
    backgroundColor.setAlpha(100);

    if(dragging) {
        QPainterPath totalPath;
        totalPath.addRect(total);
        QPainterPath exclude;
        exclude.addRect(*selection);
        auto path = (totalPath - exclude);

        painter.fillPath(path, backgroundColor);
        painter.fillRect(*selection, Qt::transparent);
        Utils::drawOutlineBox(&painter, *selection);
    } else {
        painter.fillRect(total, backgroundColor);
    }
}

void RegionGrabber::quitOnClose(bool value) {
    quit = value;
}

QRect* RegionGrabber::setSelection(QRect* selectionIn) {
    QRect old = {selectionIn->x(), selectionIn->y(), selectionIn->width(), selectionIn->height()};

    selectionIn->setX(qMin(old.x(), old.right()));
    selectionIn->setY(qMin(old.y(), old.bottom()));
    selectionIn->setRight(qMax(old.x(), old.right()));
    selectionIn->setBottom(qMax(old.y(), old.bottom()));

    delete this->selection;
    this->selection = selectionIn;
    return selectionIn;
}

QRect *RegionGrabber::getSelection() {
    return selection;
}
