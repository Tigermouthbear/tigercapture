//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_AREASCREENSHOTGRABBER_H
#define TIGERCAPTURE_AREASCREENSHOTGRABBER_H

#include <future>

#include "RegionGrabber.h"
#include "../Screenshot.h"

class AreaScreenshotGrabber: public RegionGrabber {
private:
    TigerCapture* tigerCapture;
    Screenshot* screenshot;

    std::future<void>* future = nullptr;

    bool shouldQuit = false;
    void quit();

public:
    explicit AreaScreenshotGrabber(TigerCapture* tigerCapture);

    void setQuitOnClose();

protected:
    void onFinish() override;

    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_AREASCREENSHOTGRABBER_H
