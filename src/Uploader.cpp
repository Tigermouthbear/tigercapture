//
// Created by Tigermouthbear on 8/17/20.
//

#include "Uploader.h"

#include <iostream>
#include <QtCore/QString>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QFileInfo>
#include <utility>

#include "TigerCapture.hpp"

Uploader::Uploader(TigerCapture* tigerCapture) {
    this->tigerCapture = tigerCapture;
}

std::string Uploader::Upload(const std::string& path) {
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager();
    QUrl qUrl(url.c_str());
    QNetworkRequest request(qUrl);
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // add headers
    if(headerData.size() > 0) {
        // loop all in array
        for(const std::pair<std::string, std::string>& data: headerData)
            request.setRawHeader(QByteArray::fromStdString(data.first), data.second.c_str());
    }

    // put file to form data
    QFile* file = new QFile(path.c_str());
    file->open(QIODevice::ReadOnly);
    file->setParent(multiPart);
    QFileInfo fileInfo(*file);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + QString::fromStdString(fileFormName) +"\"; filename=\"" + fileInfo.fileName() + "\""));
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setBodyDevice(file);
    multiPart->append(filePart);

    // put other data
    for(const std::pair<std::string, std::string>& data: formData) {
        QHttpPart part;
        part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(("form-data; name=\"" + data.first +"\"").c_str()));
        part.setBody(QByteArray::fromStdString(data.second));
        multiPart->append(part);
    }

    QEventLoop loop;
    QNetworkReply* reply = networkAccessManager->post(request, multiPart);
    multiPart->setParent(reply);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // wait for reply (this function shouldn't be called in the main thread)

    if(reply->error() == QNetworkReply::NoError) {
        std::string response = reply->readAll().toStdString();
        std::string out = parseVariables(responseRegex, response);
        printf("Uploaded to: %s\n", out.c_str());

        // save entry to log file
        std::ofstream log(TC::Files::getUploadsLogFile(), std::ios_base::app | std::ios_base::out);
        log << path << "," << out << "\n";
        log.close();
        printf("Saved to: %s\n", path.c_str());

        // free network request's memory
        reply->deleteLater();
        networkAccessManager->deleteLater();

        // update screenshot explorer
        tigerCapture->getMainWindow()->updateExplorer();

        return out;
    } else {
        printf("%s\n", reply->errorString().toStdString().c_str());
        reply->deleteLater();
        networkAccessManager->deleteLater();
        return "";
    }
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
