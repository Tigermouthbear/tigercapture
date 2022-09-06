//
// Created by Tigermouthbear on 9/5/22.
//

#include <QtNetwork>

class OutputSource {
public:
    virtual void write(std::string str) {
        std::cout << str;
    }

    virtual void flush() {
    }
};

class IpcOutputSource: public OutputSource {
private:
    TigerCapture* tigerCapture;
    QLocalSocket* socket;

public:
    IpcOutputSource(TigerCapture* tigerCapture, QLocalSocket* socket) {
        this->tigerCapture = tigerCapture;
        this->socket = socket;
    }

    void write(std::string str) override {
        quint16 len = str.length();
        socket->write((char*) &len, sizeof(quint16));
        socket->write(str.data(), len);
    }

    void flush() override {
        quint16 len = 0;
        socket->write((char*) &len, sizeof(quint16));
        socket->flush();
        tigerCapture->setOutput(new OutputSource());
    }
};
