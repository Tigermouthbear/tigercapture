//
// Created by GiantNuker on 8/28/20.
//

#include "PinnedAreaWindow.h"

#include <QtGui/QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <utility>
#include <QMouseEvent>
#include "../Utils.h"

PinnedAreaWindow::PinnedAreaWindow(int x, int y, QImage image): QMainWindow() {
    setWindowFlag(Qt::FramelessWindowHint);

    // make the image widget
    auto* widget = new PinnedAreaWidget(image);
    setCentralWidget(widget);

    // set position and size
    setFixedSize(image.width(), image.height());
    move(x, y);
}

void PinnedAreaWindow::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::RightButton) close();
}

PinnedAreaWindow::PinnedAreaWidget::PinnedAreaWidget(QImage image): QWidget(nullptr, Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool) {
    this->image = std::move(image);
}

void PinnedAreaWindow::PinnedAreaWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, image, 0, 0, -1, -1, Qt::AutoColor);
    Utils::drawOutlineBox(&painter, 0, 0, image.width() - 1, image.height() - 1);
}