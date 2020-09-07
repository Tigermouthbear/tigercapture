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
#include <QTimer>

RegionGrabber::RegionGrabber(): QWidget() {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    setWindowFlag(Qt::X11BypassWindowManagerHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::Tool);

    // set fullscreen
    QRect total;
    for(auto scr : QGuiApplication::screens()) {
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
    QTimer::singleShot(200, this, SLOT(grabInputs()));
}

void RegionGrabber::grabInputs() {
    grabKeyboard();
    grabMouse();
}

void RegionGrabber::closeEvent(QCloseEvent* event) {
    if(quit != nullptr) {
        delete quit;
        QApplication::quit();
    }
}

void RegionGrabber::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Escape) onFinish();
}

void RegionGrabber::updateSelection(QMouseEvent* event) {
    setSelection(new QRect(dragX, dragY, event->x() - dragX, event->y() - dragY));
}

void RegionGrabber::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragX = event->x();
        dragY = event->y();
        dragging = true;
        hasDragged = true;
        updateSelection(event);
    }
    update();
}

void RegionGrabber::mouseMoveEvent(QMouseEvent* event) {
    if(selection != nullptr) {
        auto newSelection = new QRect(dragX, dragY, event->x() - dragX, event->y() - dragY);
        auto a = *newSelection;
        auto b = *selection;

        auto combined = QRect(
                a.x() == b.x() ? a.x() : a.x() < b.x() ? a.x() - 1 : b.x() + 1,
                a.y() == b.y() ? a.y() : a.y() < b.y() ? a.y() - 1 : b.y() + 1,
                a.width() == b.width() ? a.width() : a.width() > b.width() ? a.width() + 1 : b.width() - 1,
                a.height() == b.height() ? a.height() : a.height() > b.height() ? a.height() + 1 : b.height() - 1
        ).adjusted(-1, -1, 2, 2);
        auto c = a.intersected(b);
        updateSelection(event);
        update(combined);
    } else {
        updateSelection(event);
        update();
    }
}

void RegionGrabber::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragging = false;
        updateSelection(event);
        onFinish();
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

void RegionGrabber::setQuitOnClose(Config* value) {
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

QRect* RegionGrabber::getSelection() {
    return selection;
}

void RegionGrabber::onFinish() {
    releaseKeyboard();
    releaseMouse();
    hide();
    close();
}

RegionGrabber::~RegionGrabber() {
    delete selection;
}
