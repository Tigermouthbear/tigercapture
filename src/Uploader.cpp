//
// Created by Tigermouthbear on 8/17/20.
//

#include "Uploader.h"

#include <iostream>
#include <QtCore/QString>
#include <utility>
#include "json.hpp"
#include "FileUtils.h"

Uploader::Uploader(std::string url, const std::vector<Uploader::Data>& formData, std::string fileFormName, std::string responseRegex) {
    this->url = std::move(url);
    this->formData = formData;
    this->fileFormName = std::move(fileFormName);
    this->responseRegex = std::move(responseRegex);

    curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
}

std::string Uploader::Upload(std::string& path) {
    curl_mime* form = curl_mime_init(curl);
    curl_mimepart* field = curl_mime_addpart(form);
    std::string responseBuffer;

    // put file
    curl_mime_name(field, fileFormName.c_str());
    curl_mime_filedata(field, path.c_str());

    // put other data
    for(const Uploader::Data& data: formData) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, data.key.c_str());
        curl_mime_data(field, data.value.c_str(), CURL_ZERO_TERMINATED);
    }

    // set post info
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

    CURLcode curLcode = curl_easy_perform(curl);
    curl_easy_reset(curl);

    std::cout << responseBuffer << std::endl;
    std::string out = "ERROR";
    if(curLcode == CURLE_OK) {
        QString qString = responseRegex.c_str();
        qString.replace("$response$", responseBuffer.c_str());
        out = qString.toStdString();
    }
    std::cout << out << std::endl;

    return out;
}

// https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c/9786295
size_t Uploader::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Uploader *Uploader::createFromJSON(const std::string& file) {
    nlohmann::json json = FileUtils::readJSON(file);
    std::vector<Uploader::Data> formData;
    for(auto it = json["Arguments"].begin(); it != json["Arguments"].end(); ++it) formData.emplace_back(std::string(it.key()), std::string(it.value()));

    std::cout << json["RequestURL"] << " " << json["FileFormName"] << " " << json["URL"] << std::endl;

    return new Uploader(json["RequestURL"], formData, json["FileFormName"], json["URL"]);
}

Uploader::~Uploader() {
    curl_easy_cleanup(curl);
}

Uploader::Data::Data(std::string key, std::string value) {
    this->key = std::move(key);
    this->value = std::move(value);
}
