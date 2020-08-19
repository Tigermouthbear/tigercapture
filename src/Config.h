//
// Created by Tigermouthbear on 8/18/20.
//

#ifndef TIGERCAPTURE_CONFIG_H
#define TIGERCAPTURE_CONFIG_H

#include <string>
#include "json.hpp"
#include "Uploader.h"

class Config {
private:
    std::string file;
    nlohmann::json json;

    bool minimize{};

    std::string uploaderLoc ="";
    Uploader* uploader{};

    int x;
    int y;

public:
    Config(const std::string& file);

    void read();
    void write();

    bool shouldMinimize() const;
    void setShouldMinimize(bool value);

    Uploader* getUploader();
    bool setUploader(const std::string& file);
    std::string getUploaderLoc();

    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);
};


#endif //TIGERCAPTURE_CONFIG_H
