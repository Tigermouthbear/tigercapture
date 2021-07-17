//
// Created by Tigermouthbear on 10/27/20.
//

#ifndef TIGERCAPTURE_TIGERCAPTURE_HPP
#define TIGERCAPTURE_TIGERCAPTURE_HPP

#include <string>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <utility>
#include <QtCore/QDateTime>
#include <QtGui/QPainter>
#include "json.hpp"
#include "../clip/clip.h"

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <sstream>
#include <locale>
#include <codecvt>
#else
#include <pwd.h>
#include <filesystem>
#endif

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

    static std::string isoTime() {
        QDateTime dt = QDateTime::currentDateTime();
        dt.setTimeSpec(Qt::LocalTime);
        return QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
    }

    static std::string getMonthFormatted() {
        time_t time = std::time(nullptr);
        tm* date = localtime(&time);
        std::string month = (date->tm_mon < 9 ? "0" : "") + std::to_string(date->tm_mon + 1);
        return std::to_string(1900 + date->tm_year).append("-").append(month);
    }

    //
    // Clipboard IO
    //
    namespace Clipboard {
        static void copyToClipboard(const std::string& text) {
            clip::set_text(text);
        }

        static void copyToClipboard(const QPixmap& pixmap) {
            QImage image = pixmap.toImage();

            struct clip::image_spec spec;
            spec.width = image.width();
            spec.height = image.height();
            spec.bits_per_pixel = image.depth();
            spec.bytes_per_row = image.bytesPerLine();
            spec.red_mask = 0xff0000;
            spec.green_mask = 0xff00;
            spec.blue_mask = 0xff;
            spec.alpha_mask = 0xff000000;
            spec.red_shift = 16;
            spec.green_shift = 8;
            spec.blue_shift = 0;
            spec.alpha_shift = 24;

            clip::image clip_image(image.bits(), spec);
            clip::set_image(clip_image);
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
#ifdef _WIN32
                CreateDirectory(directory.c_str(), NULL);
#else
                std::filesystem::create_directories(directory);
#endif
            }
            return directory;
        }
#ifndef _WIN32
        static std::string getHomeDirectory() {
            struct passwd* pw = getpwuid(getuid());
            return std::string(pw->pw_dir);
        }
#else
        static std::string ws2s(const std::wstring& wstr) {
            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;
            return converterX.to_bytes(wstr);
        }
#endif

        static std::string getConfigDirectory() {
#ifdef _WIN32
            wchar_t* localAppData = 0;
            SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppData);
            std::wstringstream ss;
            ss << localAppData << L"/TigerCapture";
            CoTaskMemFree(static_cast<void*>(localAppData));
            return ws2s(ss.str());
#else
            return createDirectoryIfNonexistant(getHomeDirectory() + "/.config/TigerCapture");
#endif
        }

        static std::string getLocalPicturesDirectory() {
#ifdef _WIN32
            wchar_t* picturesFolder = 0;
            SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &picturesFolder);
            std::wstringstream ss;
            ss << picturesFolder << L"/";
            CoTaskMemFree(static_cast<void*>(picturesFolder));
            return ws2s(ss.str());
#else
            return createDirectoryIfNonexistant(getHomeDirectory() + "/Pictures");
#endif
        }

        static std::string getPicturesDirectory() {
            return createDirectoryIfNonexistant(getLocalPicturesDirectory() + "/TigerCapture");
        }

        static std::string getUploadersDirectory() {
            return createDirectoryIfNonexistant(getConfigDirectory() + "/Uploaders");
        }

        static std::string getScreenshotsDir() {
            return createDirectoryIfNonexistant(getPicturesDirectory() + "/" + getMonthFormatted());
        }

        static std::string genNewImageLocation() {
            std::string path = getScreenshotsDir().append("/").append(isoTime());
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
            return createDirectoryIfNonexistant(getConfigDirectory() + "/Uploads");
        }

        static std::string getUploadsLogFile() {
            return getUploadLogsFolder() + "/" + getMonthFormatted() + ".csv";
        }

        //
        // JSON
        //
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