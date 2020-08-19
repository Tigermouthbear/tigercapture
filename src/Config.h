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

public:
    Config(const std::string& file);

    void read();
    void write();

    bool shouldMinimize() const;
    void setShouldMinimize(bool value);

    Uploader* getUploader();
    bool setUploader(const std::string& file);
    std::string getUploaderLoc();
};


#endif //TIGERCAPTURE_CONFIG_H
