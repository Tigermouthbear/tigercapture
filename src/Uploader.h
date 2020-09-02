//
// Created by Tigermouthbear on 8/17/20.
//

#ifndef TIGERCAPTURE_UPLOADER_H
#define TIGERCAPTURE_UPLOADER_H

#include <string>
#include <curl/curl.h>
#include <vector>
#include <future>
#include "json.hpp"

class Uploader {
public:
    Uploader(std::string url, const std::vector<std::pair<std::string, std::string>>& formData, std::string fileFormName, std::string responseRegex);

    std::future<void> Upload(const std::string& path, std::function<void(std::string res)> callback);

    static Uploader* createFromJSON(const std::string& file);
    static std::string findVariable(const std::string &var, const nlohmann::json &json);
    static std::string parseVariables(std::string expression, const std::string &response);
private:
    std::string url;
    std::vector<std::pair<std::string, std::string>> formData;
    std::string fileFormName;
    std::string responseRegex;

    std::string Upload(const std::string& path);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};


#endif //TIGERCAPTURE_UPLOADER_H
