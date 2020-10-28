//
// Created by Tigermouthbear on 8/17/20.
//

#include "Uploader.h"

#include <iostream>
#include <QtCore/QString>
#include <utility>
#include "json.hpp"
#include "FileUtils.h"

Uploader::Uploader(std::string url, const std::vector<std::pair<std::string, std::string>>& formData,
                   std::string fileFormName, std::string responseRegex) {
    this->url = std::move(url);
    this->formData = formData;
    this->fileFormName = std::move(fileFormName);
    this->responseRegex = std::move(responseRegex);

    curl_global_init(CURL_GLOBAL_ALL);
}

std::future<void>* Uploader::Upload(const std::string& path, void* extraData, void (* callback)(void*, const std::string&)) {
    auto* out = new std::future<void>;
    *out = std::async([=]() {
        std::string res = Upload(path);
        if(callback != nullptr) {
            callback(extraData, res);
        }
    });
    return out;
}

std::string Uploader::Upload(const std::string& path) {
    CURL* curl = curl_easy_init();
    curl_mime* form = curl_mime_init(curl);
    curl_mimepart* field = curl_mime_addpart(form);
    std::string responseBuffer;
    char errbuf[CURL_ERROR_SIZE];
    errbuf[0] = 0;

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
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

    CURLcode curLcode = curl_easy_perform(curl);

    std::string out;
    if(curLcode == CURLE_OK) {
        out = parseVariables(responseRegex, responseBuffer);
        printf("Uploaded: %s\n", out.c_str());

        // save entry to log file
        std::ofstream log(FileUtils::getUploadsLogFile(), std::ios_base::app | std::ios_base::out);
        log << path << "," << out << "\n";
        log.close();
        printf("Saved to: %s\n", path.c_str());
    } else {
        out = std::string();
        printf("ERROR uploading screenshot to %s (%d: %s) (Response: %s)\n", url.c_str(), curLcode, errbuf,
               responseBuffer.c_str());
    }

    curl_easy_cleanup(curl);
    return out;
}

std::string Uploader::findVariable(const std::string& var, const nlohmann::json& json) {
    int idx;
    if(json.contains(var)) {
        return json[var];
    } else if((idx = var.find('.')) != std::string::npos) {
        if(json.contains(var.substr(0, idx))) {
            return findVariable(var.substr(idx + 1, var.size()), json[var.substr(0, idx)]);
        } else {
            return "";
        }
    } else {
        return "";
    }
}

std::string Uploader::parseVariables(std::string expression, const std::string& response) {
    int idx = expression.size();
    while(idx != -1 && (idx = expression.rfind('$', idx)) != std::string::npos) {
        int end = idx;
        int begin = (idx = expression.rfind('$', idx - 1)) + 1;
        end -= begin;
        std::string var = expression.substr(begin, end);
        std::string res;
        if(var == "response") {
            res = response;
        } else if(var.size() > 5 && var.substr(0, 5) == "json:") {
            res = findVariable(var.substr(5, var.size()), nlohmann::json::parse(response));
        } else {
            res = "<UNKNOWN>";
        }
        expression.erase(begin - 1, end + 2);
        expression.insert(begin - 1, res);
        --idx;
    }
    return expression;
}

// https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c/9786295
size_t Uploader::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*) userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

Uploader* Uploader::createFromJSON(const std::string& file) {
    nlohmann::json json = FileUtils::readJSON(file);
    if(json == nullptr) return nullptr;

    std::string requestURL;
    std::vector<std::pair<std::string, std::string>> formData;
    std::string fileFormName;
    std::string URL;
    try {
        requestURL = json["RequestURL"];
        for(auto it = json["Arguments"].begin(); it != json["Arguments"].end(); ++it)
            formData.emplace_back(std::string(it.key()), std::string(it.value()));
        fileFormName = json["FileFormName"];
        URL = json["URL"];
    } catch(nlohmann::json::parse_error error) {
        return nullptr;
    }

    return new Uploader(requestURL, formData, fileFormName, URL);
}
