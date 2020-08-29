//
// Created by GiantNuker on 8/28/20.
//

#ifndef TIGERCAPTURE_PINNEDAREAWINDOW_H
#define TIGERCAPTURE_PINNEDAREAWINDOW_H

#include "QMainWindow"

class PinnedAreaWindow: public QMainWindow {
public:
    PinnedAreaWindow(int x, int y, QImage image, QMainWindow* parent);
    class PinnedAreaWidget: public QWidget {
    private:
        QImage image;
    public:
        PinnedAreaWidget(QImage image);
    protected:
        void paintEvent(QPaintEvent* event) override;
    };
protected:
    void mousePressEvent(QMouseEvent* event) override;
};


#endif //TIGERCAPTURE_PINNEDAREAWINDOW_H
