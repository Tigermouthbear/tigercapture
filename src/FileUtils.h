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

namespace FileUtils {
    bool exists(const std::string& path) {
        struct stat buffer{};
        return (stat (path.c_str(), &buffer) == 0);
    }

    std::string createDirectoryIfNonexistant(const std::string& directory) {
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

    std::string getHomeDirectory() {
        struct passwd* pw = getpwuid(getuid());
        return std::string(pw->pw_dir);
    }

    std::string getScreenshotsDirectory() {
        return createDirectoryIfNonexistant(getHomeDirectory() + "/TigerCapture");
    }

    std::string getDirectory() {
        time_t time = std::time(nullptr);
        tm* date = localtime(&time);
        std::string month = (date->tm_mon < 10 ? "0" : "") + std::to_string(date->tm_mon + 1);
        std::string directory = getScreenshotsDirectory().append("/").append(std::to_string(1900 + date->tm_year)).append("-").append(month);
        return createDirectoryIfNonexistant(directory);
    }

    std::string genNewImageLocation() {
        std::string directory = getDirectory();
        std::string path;

        gen:
        char tmpname[40];
        tmpnam(tmpname);
        std::string name = std::string(tmpname).substr(9);
        path = directory.append("/").append(name).append(".png");

        if(exists(path)) goto gen;

        return path;
    }
}


#endif //TIGERCAPTURE_FILEUTILS_H
