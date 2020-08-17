//
// Created by Tigermouthbear on 8/17/20.
//

#include "Uploader.h"

#include <curl/curl.h>
#include <utility>
#include <iostream>
#include <QtCore/QString>

Uploader::Uploader(std::string postURL, std::string token, std::string response) {
    this->postURL = std::move(postURL);
    this->token = std::move(token);
    this->response = std::move(response);
}

std::string Uploader::Upload(std::string& path) {
    CURL* curl = curl_easy_init();
    CURLcode res = CURLE_COULDNT_CONNECT;
    curl_mime *form;
    curl_mimepart *field;

    curl_global_init(CURL_GLOBAL_ALL);

    std::string responseBuffer;

    if(curl) {
        // create then add to form
        form = curl_mime_init(curl);
        field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, path.c_str());
        field = curl_mime_addpart(form);
        curl_mime_name(field, "token");
        curl_mime_data(field, token.c_str(), CURL_ZERO_TERMINATED);

        // set post info
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        curl_easy_setopt(curl, CURLOPT_URL, postURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // post
        res = curl_easy_perform(curl);

        // cleanup
        curl_easy_cleanup(curl);
        curl_mime_free(form);
    }

    std::cout << responseBuffer << std::endl;

    std::string out = "ERROR";
    if(res == CURLE_OK) {
        QString qString = response.c_str();
        qString.replace("$RES", responseBuffer.c_str());
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
