//
// Created by Tigermouthbear on 8/17/20.
//

#ifndef TIGERCAPTURE_UPLOADER_H
#define TIGERCAPTURE_UPLOADER_H

#include <string>

class Uploader {
private:
    std::string postURL;
    std::string token;
    std::string response;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

public:
    Uploader(std::string postURL, std::string token, std::string response);
    std::string Upload(std::string& path);
};


#endif //TIGERCAPTURE_UPLOADER_H
