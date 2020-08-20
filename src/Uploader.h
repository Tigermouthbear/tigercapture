//
// Created by Tigermouthbear on 8/17/20.
//

#ifndef TIGERCAPTURE_UPLOADER_H
#define TIGERCAPTURE_UPLOADER_H

#include <string>
#include <curl/curl.h>
#include <vector>

class Uploader {
public:
    struct Data {
        Data(std::string key, std::string value);
        std::string key;
        std::string value;
    };

    Uploader(std::string url, const std::vector<Uploader::Data>& formData, std::string fileFormName, std::string responseRegex);
    ~Uploader();

    std::string Upload(const std::string& path);

    static Uploader* createFromJSON(const std::string& file);

private:
    std::string url;
    std::vector<Uploader::Data> formData;
    std::string fileFormName;
    std::string responseRegex;

    CURL* curl;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};


#endif //TIGERCAPTURE_UPLOADER_H
