//
// Created by Tigermouthbear on 8/17/20.
//

#pragma once

#ifndef TIGERCAPTURE_UPLOADER_H
#define TIGERCAPTURE_UPLOADER_H

#include <string>
#include <curl/curl.h>
#include <vector>
#include <future>
#include "json.hpp"

class Uploader {
public:
    const static int FILE_UPLOADER = 1;
    const static int IMAGE_UPLOADER = 2;
    const static int TEXT_UPLOADER = 4;

    std::future<void>* Upload(const std::string& path, void* extraData, void (* callback)(void*, const std::string&));
    std::string Upload(const std::string& path);

    bool check(int type);

    static Uploader* createFromJSON(const std::string& file);

    static std::string findVariable(const std::string& var, const nlohmann::json& json);
    static std::string parseVariables(std::string expression, const std::string& response);

    Uploader();

    void setURL(std::string value);
    void addHeaderData(std::pair<std::string, std::string> data);
    void addFormData(std::pair<std::string, std::string> data);
    void setFileFormName(std::string value);
    void setResponseRegex(std::string value);
    void setType(int value);

private:
    int type = 0;

    std::string url;
    std::vector<std::pair<std::string, std::string>> headerData;
    std::vector<std::pair<std::string, std::string>> formData;
    std::string fileFormName;
    std::string responseRegex;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};


#endif //TIGERCAPTURE_UPLOADER_H
