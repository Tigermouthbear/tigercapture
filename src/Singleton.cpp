//
// Created by Tigermouthbear on 12/12/20.
//

#include "Singleton.h"

// adapted from: https://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c-or-c
Singleton::Singleton(uint16_t port0): socket_fd(-1), rc(1), port(port0)  {
}

bool Singleton::operator()() {
    if(socket_fd == -1 || rc) {
        socket_fd = -1;
        rc = 1;

        if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            throw std::runtime_error(std::string("Could not create socket: ") +  strerror(errno));
        }
        else {
            struct sockaddr_in name;
            name.sin_family = AF_INET;
            name.sin_port = htons (port);
            name.sin_addr.s_addr = htonl (INADDR_ANY);
            rc = bind (socket_fd, (struct sockaddr *) &name, sizeof (name));
        }
    }
    return (socket_fd != -1 && rc == 0);
}

Singleton::~Singleton() {
    if(socket_fd != -1) shutdown(socket_fd, 2/*SHUT_RDWR*/);
}
