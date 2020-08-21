//
// Created by Tigermouthbear on 8/20/20.
//

#include "DragUploadWidget.h"
#include "../Utils.h"

#include <QtGui/QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QPainter>

DragUploadWidget::DragUploadWidget(Config* config): QWidget(nullptr, Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool) {
    this->config = config;

    resize(100, 100);
    setAcceptDrops(true);
}

// start drag on click
void DragUploadWidget::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        dragX = event->globalX() - x;
        dragY = event->globalY() - y;
        dragging = true;
    } else if(event->button() == Qt::RightButton) close();
}

// move widget with mouse while dragging
void DragUploadWidget::mouseMoveEvent(QMouseEvent* event) {
    if(dragging) {
        x = event->globalX() - dragX;
        y = event->globalY() - dragY;
        move(x, y);
    }
}

void DragUploadWidget::mouseReleaseEvent(QMouseEvent* event) {
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

void DragUploadWidget::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasUrls()) event->acceptProposedAction();
}

void DragUploadWidget::dropEvent(QDropEvent *event) {
    const QMimeData* mimeData = event->mimeData();

    if(mimeData->hasUrls() && mimeData->urls().size() == 1) {
        QUrl url = mimeData->urls().at(0);
        if(config->getUploader() != nullptr && url.isLocalFile()) {
            auto upload = config->getUploader()->Upload(url.toLocalFile().toStdString(), [&](const std::string& res) {
                if(res.empty()) return;

                // copy response
                auto clip = QApplication::clipboard();
                clip->setText(res.c_str());

                // display notification
                config->getSystemTrayIcon()->showMessage("TigerCapture", ("Uploaded to: " + res).c_str());
            });
        }
    }
}

void DragUploadWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawText(0, 0, width(), height(), Qt::AlignHCenter | Qt::AlignVCenter, "Drag and Drop");
    Utils::drawOutlineBox(&painter, 0, 0, width() - 1, height() - 1);
}
