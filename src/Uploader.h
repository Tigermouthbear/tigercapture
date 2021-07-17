//
// Created by Tigermouthbear on 8/17/20.
//

#ifndef TIGERCAPTURE_UPLOADER_H
#define TIGERCAPTURE_UPLOADER_H

#include <string>
#include <vector>
#include <future>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QUrlQuery>
#include <QUrl>
#include "json.hpp"

class MainWindow;
class Config;
class UploadsExplorerWidget;
class AreaScreenshotGrabber;
class PinnedAreaGrabber;
class Uploader;
#include "TigerCapture.h"
class TigerCapture;

class Uploader: QObject {
Q_OBJECT
public:
    const static int FILE_UPLOADER = 1;
    const static int IMAGE_UPLOADER = 2;
    const static int TEXT_UPLOADER = 4;

    std::string Upload(const std::string& path);

    bool check(int type);

    static Uploader* createFromJSON(TigerCapture* tigerCapture, const std::string& file);

    static std::string findVariable(const std::string& var, const nlohmann::json& json);
    static std::string parseVariables(std::string expression, const std::string& response);

    Uploader(TigerCapture* tigerCapture);

    void setURL(std::string value);
    void addHeaderData(std::pair<std::string, std::string> data);
    void addFormData(std::pair<std::string, std::string> data);
    void setFileFormName(std::string value);
    void setResponseRegex(std::string value);
    void setType(int value);

private:
    TigerCapture* tigerCapture;
    int type = 0;

    std::string url;
    std::vector<std::pair<std::string, std::string>> headerData;
    std::vector<std::pair<std::string, std::string>> formData;
    std::string fileFormName;
    std::string responseRegex;
};


#endif //TIGERCAPTURE_UPLOADER_H
