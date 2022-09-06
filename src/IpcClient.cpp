//
// Created by Tigermouthbear on 9/3/22.
//

#include "IpcClient.h"

IpcClient::IpcClient(std::function<void(char*, quint16)> callback) {
    this->callback = callback;
    socket = new QLocalSocket(this);
    socket->connectToServer("tigercapture");
    alive = socket->waitForConnected();
}

bool IpcClient::isAlive() {
    return alive;
}

void IpcClient::sendSignal(qint8 signal, char* str) {
    socket->write((char*) &signal, sizeof(qint8));

    quint16 len = str == 0 ? 0 : strlen(str);
    socket->write((char*) &len, sizeof(quint16));
    if(len > 0) socket->write(str, len);

    socket->flush();
}

void IpcClient::waitForRead() {
    while(true) {
        quint16 len;
        while(socket->bytesAvailable() < sizeof(quint16)) {
            socket->waitForReadyRead();
        }
        socket->read((char*) &len, sizeof(quint16));

        if(len == 0) return;

        char str[len + 1];
        while(socket->bytesAvailable() < len) {
            socket->waitForReadyRead();
        }
        socket->read(str, len);
        str[len] = 0;

        callback(str, len);
    }
}


IpcClient::~IpcClient() {
    socket->disconnectFromServer();
    delete socket;
}
