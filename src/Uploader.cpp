//
// Created by Tigermouthbear on 8/17/20.
//

#include "Uploader.h"

#include <iostream>
#include <QSystemTrayIcon>
#include <QtCore/QString>
#include <utility>
#include "json.hpp"
#include "FileUtils.h"

Uploader::Uploader(std::string url, const std::vector<std::pair<std::string, std::string>>& formData, std::string fileFormName, std::string responseRegex) {
    this->url = std::move(url);
    this->formData = formData;
    this->fileFormName = std::move(fileFormName);
    this->responseRegex = std::move(responseRegex);

    curl_global_init(CURL_GLOBAL_ALL);
}
std::future<void> Uploader::Upload(const std::string &path, std::function<void(std::string res)> callback) {
    return std::async([&]() {
        std::string res = Upload(path);
        if(callback) callback(res);
    });
}

std::string Uploader::Upload(const std::string& path) {
    CURL* curl = curl_easy_init();
    curl_mime* form = curl_mime_init(curl);
    curl_mimepart* field = curl_mime_addpart(form);
    std::string responseBuffer;

    // put file
    curl_mime_name(field, fileFormName.c_str());
    curl_mime_filedata(field, path.c_str());

    // put other data
    for(const std::pair<std::string, std::string>& data: formData) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, data.first.c_str());
        curl_mime_data(field, data.second.c_str(), CURL_ZERO_TERMINATED);
    }

    // set post info
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

    CURLcode curLcode = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::string out;
    if(curLcode == CURLE_OK) {
        QString qString = responseRegex.c_str();
        qString.replace("$response$", responseBuffer.c_str());
        out = qString.toStdString();
        printf("Uploaded: %s\n", out.c_str());
    } else printf("ERROR uploading screenshot to %s (%d)\n", url.c_str(), curLcode);

    // save entry to log file
    std::ofstream log(FileUtils::getUploadsLogFile(), std::ios_base::app | std::ios_base::out);
    log << path << "," << out << "\n";
    log.close();
    printf("Saved to: %s\n", path.c_str());

    return out;
}

// https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c/9786295
size_t Uploader::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Uploader *Uploader::createFromJSON(const std::string& file) {
    nlohmann::json json = FileUtils::readJSON(file);
    if(json == nullptr) return nullptr;

    std::string requestURL;
    std::vector<std::pair<std::string, std::string>> formData;
    std::string fileFormName;
    std::string URL;
    try {
        requestURL = json["RequestURL"];
        for(auto it = json["Arguments"].begin(); it != json["Arguments"].end(); ++it) formData.emplace_back(std::string(it.key()), std::string(it.value()));
        fileFormName = json["FileFormName"];
        URL = json["URL"];
    } catch(nlohmann::json::parse_error error) {
        return nullptr;
    }

    return new Uploader(requestURL, formData, fileFormName, URL);
}