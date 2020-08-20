//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_REGIONGRABBER_H
#define TIGERCAPTURE_REGIONGRABBER_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>

class RegionGrabber: public QWidget {
Q_OBJECT
private:
    bool quit = false;
    QRect* selection = nullptr;

    QRect* setSelection(QRect* selectionIn);

public:
    RegionGrabber();

    void quitOnClose(bool value);
    QRect* getSelection();

protected:
    bool dragging = false;
    bool hasDragged = false;
    int dragX = 0;
    int dragY = 0;

    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_REGIONGRABBER_H
