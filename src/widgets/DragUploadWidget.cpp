//
// Created by Tigermouthbear on 8/20/20.
//

#include <QtGui/QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QMimeData>
#include <QApplication>
#include <QPainter>

#include "DragUploadWidget.h"

DragUploadWidget::DragUploadWidget(TigerCapture* tigerCapture): QWidget(nullptr, Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool) {
    this->tigerCapture = tigerCapture;

    setAttribute(Qt::WA_DeleteOnClose);

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
    if(dragging) {
        // If this widget is out of all screen bounds then close
        bool visible = false;
        int right = x + width();
        int bottom = y + height();
        for(auto scr : QGuiApplication::screens()) {
            auto g = scr->geometry();
            if(!(x < g.right() && right > g.left() && y > g.bottom() && bottom < g.top())) {
                visible = true;
                break;
            }
        }
        if(!visible) close();
        if(event->button() == Qt::LeftButton) dragging = false;
    }
}

void DragUploadWidget::dragEnterEvent(QDragEnterEvent* event) {
    if(event->mimeData()->hasUrls()) event->acceptProposedAction();
}

void DragUploadWidget::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();

    if(mimeData->hasUrls() && mimeData->urls().size() == 1) {
        QUrl url = mimeData->urls().at(0);
        if(tigerCapture->getConfig()->getUploader() != nullptr && url.isLocalFile()) {
            QString file = url.toLocalFile();

            // make sure it can upload
            if(file.endsWith(".txt")) {
                if(!tigerCapture->getConfig()->getUploader()->check(Uploader::TEXT_UPLOADER)) return;
            } else if(file.endsWith(".png") || file.endsWith(".jpg") || file.endsWith(".jpeg") || file.endsWith(".gif")) {
                if(!tigerCapture->getConfig()->getUploader()->check(Uploader::IMAGE_UPLOADER)) return;
            } else {
                if(!tigerCapture->getConfig()->getUploader()->check(Uploader::FILE_UPLOADER)) return;
            }

            auto* out = new std::future<void>;
            *out = std::async([=]() {
                // clear so user doesnt accidentally paste something else while waiting for the image to upload
                TC::Clipboard::clearClipboard();

                // actually upload
                std::string res = tigerCapture->getConfig()->getUploader()->Upload(file.toStdString());
                if(res.empty()) return;

                // copy response
                TC::Clipboard::copyToClipboard(res);

                // display notification
                ((TigerCapture*) tigerCapture)->getSystemTray()->showMessage("TigerCapture", ("Uploaded to: " + res).c_str());
            });
            // discard future, just let this run in the background
        }
    }
}

void DragUploadWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawText(0, 0, width(), height(), Qt::AlignHCenter | Qt::AlignVCenter, "Drag and Drop");
    TC::drawOutlineBox(&painter, 0, 0, width() - 1, height() - 1);
}
