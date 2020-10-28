//
// Created by Tigermouthbear on 10/27/20.
//

#ifndef TIGERCAPTURE_TIGERCAPTURE_HPP
#define TIGERCAPTURE_TIGERCAPTURE_HPP

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
#include <QtGui/QPainter>
#include "../clip/clip.h"
#include "json.hpp"

namespace TC {
    //
    // Misc
    //
    static inline void drawOutlineBox(QPainter* painter, int x, int y, int width, int height) {
        painter->drawLine(x, y, x + width, y);
        painter->drawLine(x + width, y, x + width, y + height);
        painter->drawLine(x + width, y + height, x, y + height);
        painter->drawLine(x, y + height, x, y);
    }

    static inline void drawOutlineBox(QPainter* painter, QRect box) {
        drawOutlineBox(painter, box.x(), box.y(), box.width(), box.height());
    }

    //
    // Clipboard IO
    //
    namespace Clipboard {
        static void copyToClipboard(const std::string& text) {
            auto cstr = text.c_str();
            clip::set_text(cstr);
        }

        static void copyToClipboard(const QPixmap& pixmap) {
            auto image = pixmap.toImage();

            clip::image_spec spec = {
                    (unsigned long) image.width(),
                    (unsigned long) image.height(),
                    (unsigned long) image.depth(),
                    (unsigned long) image.bytesPerLine(),
                    0xff, // red
                    0xff00, // green
                    0xff0000, // blue
                    0xff000000,
                    0,
                    8,
                    16,
                    24
            };
            clip::image img(image.data_ptr(), spec);
            clip::set_image(img);
        }

        static void clearClipboard() {
            clip::clear();
        }
    }

    //
    // File IO
    //
    namespace Files {
        static bool exists(const std::string& path) {
            struct stat buffer{};
            return (stat(path.c_str(), &buffer) == 0);
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
            while(exists(final)) {
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
    }
}

#endif //TIGERCAPTURE_TIGERCAPTURE_HPP