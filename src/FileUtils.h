//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_FILEUTILS_H
#define TIGERCAPTURE_FILEUTILS_H

#include <string>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <pwd.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <utility>
#include <QtCore/QDateTime>
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
        return createDirectoryIfNonexistant(getHomeDirectory() + "/.config/TigerCapture");
    }

    static std::string getLocalPicturesDirectory() {
        return createDirectoryIfNonexistant(getHomeDirectory() + "/Pictures");
    }

    static std::string getPicturesDirectory() {
        return createDirectoryIfNonexistant(getLocalPicturesDirectory() + "/TigerCapture");
    }

    static std::string getUploadersDirectory() {
        return createDirectoryIfNonexistant(getApplicationDirectory() + "/Uploaders");
    }

    static std::string isoTime() {
        QDateTime dt = QDateTime::currentDateTime();
        dt.setTimeSpec(Qt::LocalTime);
        return QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
    }

    static std::string genNewImageLocation() {
        std::string path = getPicturesDirectory().append("/").append(isoTime());
        std::string final = path;
        final.append(".png");

        // If the file already exists append an incremental value
        int i = 1;
        while (exists(final)) {
            final = path;
            final.append("_").append(std::to_string(i++)).append(".png");
        }

        return final;
    }

    static std::string getUploadLogsFolder() {
        return createDirectoryIfNonexistant(getApplicationDirectory() + "/Uploads");
    }

    static std::string getMonthFormatted() {
        time_t time = std::time(nullptr);
        tm* date = localtime(&time);
        std::string month = (date->tm_mon < 10 ? "0" : "") + std::to_string(date->tm_mon + 1);
        return std::to_string(1900 + date->tm_year).append("-").append(month);
    }

    static std::string getUploadsLogFile() {
        return getUploadLogsFolder() + "/" + getMonthFormatted() + ".csv";
    }

    static nlohmann::json readJSON(const char* file) {
        if(exists(file)) {
            try {
                std::ifstream ifs(file);
                return nlohmann::json::parse(ifs);
            } catch(nlohmann::json::parse_error) {
            }
        }
        return nullptr;
    }

    static inline nlohmann::json readJSON(const std::string& file) {
        return readJSON(file.c_str());
    }

    static void writeJSON(const char* file, const nlohmann::json& json) {
        std::ofstream ofstream(file);
        ofstream << json;
        ofstream.close();
    }

    static inline void writeJSON(const std::string& file, const nlohmann::json& json) {
        writeJSON(file.c_str(), json);
    }
};


#endif //TIGERCAPTURE_FILEUTILS_H
