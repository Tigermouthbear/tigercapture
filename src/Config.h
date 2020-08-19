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
    bool upload{};
    Uploader* uploader{};

public:
    Config(const std::string& file);

    void Read();
    void Write();

    bool shouldMinimize() const;
    void setShouldMinimize(bool value);

    bool shouldUpload() const;
    void setShouldUpload(bool value);

    Uploader* getUploader();
};


#endif //TIGERCAPTURE_CONFIG_H
