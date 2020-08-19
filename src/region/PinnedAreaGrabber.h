//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_PINNEDAREAGRABBER_H
#define TIGERCAPTURE_PINNEDAREAGRABBER_H

#include "RegionGrabber.h"
#include "../Screenshot.h"

class PinnedAreaGrabber: public RegionGrabber {
private:
    Screenshot* screenshot;

public:
    PinnedAreaGrabber(Config* config);

protected:
    void closeEvent(QCloseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};


#endif //TIGERCAPTURE_PINNEDAREAGRABBER_H
