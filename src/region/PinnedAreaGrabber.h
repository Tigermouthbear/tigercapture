//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_PINNEDAREAGRABBER_H
#define TIGERCAPTURE_PINNEDAREAGRABBER_H

#include "RegionGrabber.h"
#include "../Screenshot.h"

class PinnedAreaGrabber: public RegionGrabber {
private:
    TigerCapture* tigerCapture;
    Screenshot* screenshot;

public:
    explicit PinnedAreaGrabber(TigerCapture* tigerCapture);

    void open();

protected:
    void onFinish() override;

    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_PINNEDAREAGRABBER_H
