//
// Created by Tigermouthbear on 9/11/20.
//

#ifndef TIGERCAPTURE_SYSTEMTRAY_H
#define TIGERCAPTURE_SYSTEMTRAY_H

#include <QtWidgets/QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class SystemTray: public QSystemTrayIcon {
Q_OBJECT
private:
    QMenu* menu;
    QAction* exitAction;

public:
    explicit SystemTray(const std::string& icon);

private slots:
    static void exit();
};


#endif //TIGERCAPTURE_SYSTEMTRAY_H
