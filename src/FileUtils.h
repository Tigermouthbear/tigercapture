//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_FILEUTILS_H
#define TIGERCAPTURE_FILEUTILS_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <fstream>
#include <utility>
#include "json.hpp"

class FileUtils {
public:
    static bool exists(const std::string& path) {
        struct stat buffer{};
        return (stat (path.c_str(), &buffer) == 0);
    }

    static std::string createDirectoryIfNonexistant(const std::string& directory) {
        if(!exists(directory)) {
            mode_t nMode = 0733;
            #if defined(_WIN32)
            _mkdir(directory.c_str());
            #else
            mkdir(directory.c_str(), nMode);
            #endif
        }
        return directory;
    }

    static std::string getHomeDirectory() {
        struct passwd* pw = getpwuid(getuid());
        return std::string(pw->pw_dir);
    }

    static std::string getApplicationDirectory() {
        return createDirectoryIfNonexistant(getHomeDirectory() + "/TigerCapture");
    }

    static std::string getCurrentScreenshotDirectory() {
        time_t time = std::time(nullptr);
        tm* date = localtime(&time);
        std::string month = (date->tm_mon < 10 ? "0" : "") + std::to_string(date->tm_mon + 1);
        std::string directory = getApplicationDirectory().append("/").append(std::to_string(1900 + date->tm_year)).append("-").append(month);
        return createDirectoryIfNonexistant(directory);
    }

    static std::string genNewImageLocation() {
        std::string directory = getCurrentScreenshotDirectory();
        std::string path;

        gen:
        char tmpname[40];
        tmpnam(tmpname);
        std::string name = std::string(tmpname).substr(9);
        path = directory.append("/").append(name).append(".png");

        if(exists(path)) goto gen;

        return path;
    }

    static nlohmann::json readJSON(const char* file, nlohmann::json defaulT) {
        if(exists(file)) {
            try {
                std::ifstream ifs(file);
                return nlohmann::json::parse(ifs);
            } catch(nlohmann::json::parse_error error) {
                std::cout << error.what() << std::endl;
            }
        }

        return defaulT;
    }

    static inline nlohmann::json readJSON(const std::string& file, nlohmann::json defaulT) {
        return readJSON(file.c_str(), std::move(defaulT));
    }

    static void writeJSON(const char* file, nlohmann::json json) {
        std::ofstream ofstream(file);
        ofstream << json;
        ofstream.close();
    }

    static inline void writeJSON(const std::string& file, nlohmann::json json) {
        writeJSON(file.c_str(), std::move(json));
    }
};


#endif //TIGERCAPTURE_FILEUTILS_H
