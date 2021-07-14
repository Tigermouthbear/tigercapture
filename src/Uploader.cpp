//
// Created by Tigermouthbear on 8/17/20.
//

#include "Uploader.h"

#include <iostream>
#include <QtCore/QString>
#include <utility>

#include "TigerCapture.hpp"

Uploader::Uploader(TigerCapture* tigerCapture) {
    this->tigerCapture = tigerCapture;
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
    std::string responseBuffer;
    char errbuf[CURL_ERROR_SIZE];
    errbuf[0] = 0;


    // add headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "content-type: multipart/form-data;");
    if(headerData.size() > 0) {
        // loop all in array
        for(const std::pair<std::string, std::string>& data: headerData)
            headers = curl_slist_append(headers, (data.first + ": " + data.second).c_str());
    }

    // add them to request
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // add arguments
    curl_mime* form = curl_mime_init(curl);
    curl_mimepart* field = curl_mime_addpart(form);

    // put file
    curl_mime_name(field, fileFormName.c_str());
    curl_mime_filedata(field, path.c_str());

    // put other data
    for(const std::pair<std::string, std::string>& data: formData) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, data.first.c_str());
        curl_mime_data(field, data.second.c_str(), CURL_ZERO_TERMINATED);
    }

    // add to request
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);


    // set post info
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

    // post
    CURLcode curLcode = curl_easy_perform(curl);

    // parse response
    std::string out;
    if(curLcode == CURLE_OK) {
        out = parseVariables(responseRegex, responseBuffer);
        printf("Uploaded to: %s\n", out.c_str());

        // save entry to log file
        std::ofstream log(TC::Files::getUploadsLogFile(), std::ios_base::app | std::ios_base::out);
        log << path << "," << out << "\n";
        log.close();
        printf("Saved to: %s\n", path.c_str());
    } else {
        out = std::string();
        printf("ERROR uploading screenshot to %s (%d: %s) (Response: %s)\n", url.c_str(), curLcode, errbuf,
               responseBuffer.c_str());
    }

    // free mem
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_mime_free(form);

    // update screenshot explorer
    tigerCapture->getMainWindow()->updateExplorer();

    return out;
}

void Uploader::setURL(std::string value) {
    url = value;
}

void Uploader::addHeaderData(std::pair<std::string, std::string> data) {
    headerData.push_back(data);
}

void Uploader::addFormData(std::pair<std::string, std::string> data) {
    formData.push_back(data);
}

void Uploader::setFileFormName(std::string value) {
    fileFormName = value;
}

void Uploader::setResponseRegex(std::string value) {
    responseRegex = value;
}

void Uploader::setType(int value) {
    type = value;
}

bool Uploader::check(int typeIn) {
    return (type & typeIn) == typeIn;
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

Uploader* Uploader::createFromJSON(TigerCapture* tigerCapture, const std::string& file) {
    nlohmann::json json = TC::Files::readJSON(file);
    if(json == nullptr) return nullptr;

    Uploader* uploader = new Uploader(tigerCapture);
    try {
        nlohmann::json::array_t destination = json["DestinationType"];
        for(auto it = destination.begin(); it != destination.end(); it++) {
            std::string name = it->get<std::string>();
            std::transform(name.begin(), name.end(), name.begin(), ::toupper);

            if(name == "FILEUPLOADER") uploader->type = uploader->type | FILE_UPLOADER;
            else if(name == "IMAGEUPLOADER") {
                uploader->type = uploader->type | IMAGE_UPLOADER;
            }
            else if(name == "TEXTUPLOADER") uploader->type = uploader->type | TEXT_UPLOADER;
        }

        uploader->url = json["RequestURL"];

        if(json.contains("Headers")) {
            nlohmann::json args = json["Headers"];
            for(auto it = args.begin(); it != args.end(); ++it)
                uploader->headerData.emplace_back(std::string(it.key()), std::string(it.value()));
        }

        if(json.contains("Arguments")) {
            nlohmann::json args = json["Arguments"];
            for(auto it = args.begin(); it != args.end(); ++it)
                uploader->formData.emplace_back(std::string(it.key()), std::string(it.value()));
        }

        uploader->fileFormName = json["FileFormName"];

        uploader->responseRegex = json["URL"];
    } catch(nlohmann::json::parse_error error) {
        return nullptr;
    }

    return uploader;
}
