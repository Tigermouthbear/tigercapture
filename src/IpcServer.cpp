//
// Created by Tigermouthbear on 9/3/22.
//

#include "IpcServer.h"

#include <iostream>
#include <sstream>

IpcServer::IpcServer(TigerCapture* tigerCapture, std::function<void(qint8, char*)> callback) {
    this->tigerCapture = tigerCapture;
    this->callback = callback;
    server = new QLocalServer(this);
    alive = server->listen("tigercapture");

    if(alive) {
        QObject::connect(server, SIGNAL(newConnection()), this, SLOT(connection()));
    }
}

bool IpcServer::isAlive() {
    return alive;
}

void IpcServer::connection() {
    QLocalSocket* socket = server->nextPendingConnection();

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(receive()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void IpcServer::receive() {
    QLocalSocket* socket = static_cast<QLocalSocket*>(sender());
    if(socket) {
        qint8 sig;
        quint16 len;
        while(socket->bytesAvailable() < sizeof(qint8) + sizeof(quint16)) {
            socket->waitForReadyRead();
        }

        socket->read((char*) &sig, sizeof(qint8));
        socket->read((char*) &len, sizeof(quint16));

        char str[len + 1];
        if(len > 0) {
            while(socket->bytesAvailable() < len) {
                socket->waitForReadyRead();
            }
            socket->read(str, len);
        }
        str[len] = 0;

        tigerCapture->setOutput(new IpcOutputSource(tigerCapture, socket));
        callback(sig, str);
    }
}

void IpcServer::handleSignal(qint8 sig, char* str) {
    callback(sig, str);
}

IpcServer::~IpcServer() {
    server->close();
    delete server;
}
