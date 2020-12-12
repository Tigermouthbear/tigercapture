//
// Created by Tigermouthbear on 12/12/20.
//

#ifndef TIGERCAPTURE_SINGLETON_H
#define TIGERCAPTURE_SINGLETON_H


#include <netinet/in.h>
#include <string>
#include <cstring>
#include <stdexcept>

// adapted from: https://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c-or-c
class Singleton {
private:
    int socket_fd = -1;
    int rc;
    uint16_t port;

public:
    Singleton(uint16_t port0);

    ~Singleton();

    bool operator()();
};


#endif //TIGERCAPTURE_SINGLETON_H
