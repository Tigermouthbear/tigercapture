//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_PINNEDAREA_H
#define TIGERCAPTURE_PINNEDAREA_H

#include <QWidget>

class PinnedArea: public QWidget {
Q_OBJECT
private:
    QImage image;

    int dragX;
    int dragY;
    bool dragging = false;
    int x = 0;
    int y = 0;
    int screenWidth;
    int screenHeight;

public:
    explicit PinnedArea(QImage image);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_PINNEDAREA_H
