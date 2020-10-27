//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_SCREENSHOT_H
#define TIGERCAPTURE_SCREENSHOT_H

#include <future>
#include <ctime>
#include <string>
#include <QPixmap>

class Screenshot;
#include "TigerCapture.h"
class TigerCapture;

#include "Uploader.h"

class Screenshot {
private:
    TigerCapture* tigerCapture;

    QPixmap pixmap;

public:
    Screenshot(TigerCapture* tigerCapture);

    ~Screenshot();

    void take();

    void crop(int x, int y, int width, int height);

    std::future<void>* save();

    QImage image();
};


#endif //TIGERCAPTURE_SCREENSHOT_H
