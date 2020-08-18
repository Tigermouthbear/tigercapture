//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_UTILS_H
#define TIGERCAPTURE_UTILS_H

#include <QPainter>

class Utils {
public:
    static inline int clamp(int value, int min, int max) {
        if(value > max) value = max;
        if(value < min) value = min;
        return value;
    }

    static inline void drawOutlineBox(QPainter* painter, int x, int y, int width, int height) {
        painter->drawLine(x, y, x + width, y);
        painter->drawLine(x + width, y, x + width, y + height);
        painter->drawLine(x + width, y + height, x, y + height);
        painter->drawLine(x, y + height, x, y);
    }

    static inline void drawOutlineBox(QPainter* painter, int x, int y, int width, int height, const char* color) {
        QPen penHText((QColor(color)));
        painter->setPen(penHText);
        drawOutlineBox(painter, x, y, width, height);
    }
};


#endif //TIGERCAPTURE_UTILS_H
