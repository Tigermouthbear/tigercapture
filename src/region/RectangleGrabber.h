//
// Created by cook on 8/20/20.
//

#ifndef TIGERCAPTURE_RECTANGLEGRABBER_H
#define TIGERCAPTURE_RECTANGLEGRABBER_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>

class RectangleGrabber: public QWidget {
protected:
    int mouseX = 0;
    int mouseY = 0;

    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_RECTANGLEGRABBER_H
