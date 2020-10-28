//
// Created by Tigermouthbear on 9/11/20.
//

#ifndef TIGERCAPTURE_SYSTEMTRAY_H
#define TIGERCAPTURE_SYSTEMTRAY_H

#include <QtWidgets/QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class SystemTray;
#include "../TigerCapture.h"
class TigerCapture;

class SystemTray: public QSystemTrayIcon {
Q_OBJECT
private:
    TigerCapture* tigerCapture;
    QMenu* menu;
    QAction* exitAction;

public:
    explicit SystemTray(TigerCapture* tigerCapture, const std::string& icon);

private slots:
    void openWindow();

    static void exit();
};


#endif //TIGERCAPTURE_SYSTEMTRAY_H
