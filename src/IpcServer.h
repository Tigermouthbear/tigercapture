//
// Created by Tigermouthbear on 9/3/22.
//

#ifndef TIGERCAPTURE_IPCSERVER_H
#define TIGERCAPTURE_IPCSERVER_H

#include "TigerCapture.h"
#include <QtNetwork>

class IpcServer: QObject {
Q_OBJECT
private:
    TigerCapture* tigerCapture;
    QLocalServer* server;
    std::function<void(qint8, char*)> callback;
    bool alive;

private slots:
    void connection();
    void receive();

public:
    IpcServer(TigerCapture* tigerCapture, std::function<void(qint8, char*)> callback);
    
    bool isAlive();

    void handleSignal(qint8 sig, char* str);

    ~IpcServer();
};


#endif //TIGERCAPTURE_IPCSERVER_H
