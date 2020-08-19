//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_AREASCREENSHOTGRABBER_H
#define TIGERCAPTURE_AREASCREENSHOTGRABBER_H


#include "RegionGrabber.h"
#include "../Screenshot.h"

class AreaScreenshotGrabber: public RegionGrabber {
private:
    Screenshot* screenshot;

public:
    AreaScreenshotGrabber(Config* config);

protected:
    void closeEvent(QCloseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_AREASCREENSHOTGRABBER_H
