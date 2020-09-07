//
// Created by Tigermouthbear on 8/20/20.
//

#ifndef TIGERCAPTURE_DRAGUPLOADWIDGET_H
#define TIGERCAPTURE_DRAGUPLOADWIDGET_H

#include <QWidget>
#include "../Config.h"

class DragUploadWidget: public QWidget {
private:
    Config* config;

    int dragX{};
    int dragY{};
    bool dragging = false;
    int x = 0;
    int y = 0;

public:
    explicit DragUploadWidget(Config* config);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;

    void dropEvent(QDropEvent* event) override;

    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_DRAGUPLOADWIDGET_H
