//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_SCREENSHOT_H
#define TIGERCAPTURE_SCREENSHOT_H

#include <ctime>
#include <string>
#include <QPixmap>
#include "Config.h"
#include "Uploader.h"

class Screenshot {
private:
    Config* config;

    QPixmap pixmap;

public:
    Screenshot(Config* config);
    ~Screenshot();

    void take();
    void take(int x, int y, int width, int height);
    void crop(int x, int y, int width, int height);
    void save();
    QImage image();
};


#endif //TIGERCAPTURE_SCREENSHOT_H
