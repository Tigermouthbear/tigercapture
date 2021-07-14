//
// Created by Tigermouthbear on 10/26/20.
//

#ifndef TIGERCAPTURE_CONFIG_H
#define TIGERCAPTURE_CONFIG_H

#include <string>
#include "json.hpp"
#include "Uploader.h"

class MainWindow;
class Config;
class UploadsExplorerWidget;
class AreaScreenshotGrabber;
class PinnedAreaGrabber;
class Uploader;
#include "TigerCapture.h"
class TigerCapture;

class Config {
private:
    TigerCapture* tigerCapture;

    const char* file;
    nlohmann::json json;

    bool minimize{};

    bool clipboard{};

    std::string uploaderLoc;
    Uploader* uploader{};

    int x{};
    int y{};

    int delay{};

    Uploader* createFromJSON(const std::string& file);

public:
    Config(TigerCapture* tigerCapture, const char* path);

    void read();
    void write();

    bool shouldMinimize() const;
    void setShouldMinimize(bool value);

    bool shouldClipboard() const;
    void setShouldClipboard(bool value);

    Uploader* getUploader();
    bool setUploader(const std::string& file);
    std::string getUploaderLoc();

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    int getDelay() const;
    void setDelay(int delayIn);
};


#endif //TIGERCAPTURE_CONFIG_H
