//
// Created by Tigermouthbear on 8/20/20.
//

#ifndef TIGERCAPTURE_DRAGUPLOADWIDGET_H
#define TIGERCAPTURE_DRAGUPLOADWIDGET_H

#include <QWidget>

class DragUploadWidget;
#include "../TigerCapture.h"
class TigerCapture;

class DragUploadWidget: public QWidget {
private:
    TigerCapture* tigerCapture;

    int dragX{};
    int dragY{};
    bool dragging = false;
    int x = 0;
    int y = 0;

public:
    explicit DragUploadWidget(TigerCapture* tigerCapture);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;

    void dropEvent(QDropEvent* event) override;

    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_DRAGUPLOADWIDGET_H
