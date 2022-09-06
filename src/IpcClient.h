//
// Created by Tigermouthbear on 9/3/22.
//

#ifndef TIGERCAPTURE_IPCCLIENT_H
#define TIGERCAPTURE_IPCCLIENT_H

#include <QtNetwork>

class IpcClient: QObject {
Q_OBJECT
private:
    std::function<void(char*, quint16)> callback;
    QLocalSocket* socket;
    bool alive;

public:
    IpcClient(std::function<void(char*, quint16)> callback);

    bool isAlive();

    void sendSignal(qint8 sig, char* str);

    void waitForRead();
    
    ~IpcClient();
};


#endif //TIGERCAPTURE_IPCCLIENT_H
