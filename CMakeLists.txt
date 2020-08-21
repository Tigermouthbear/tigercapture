cmake_minimum_required(VERSION 3.17)
project(tigercapture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lcurl -pthread")

set(CMAKE_INSTALL_PREFIX /usr/bin)

# Tell cmake where Qt is located
set(Qt5_DIR "/usr/lib/cmake/Qt5/")

# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# automoc!
set(CMAKE_AUTOMOC ON)

add_executable(tigercapture
        src/main.cpp
        src/region/AreaScreenshotGrabber.cpp src/region/AreaScreenshotGrabber.h
        src/region/PinnedAreaGrabber.cpp src/region/PinnedAreaGrabber.h
        src/region/RegionGrabber.cpp src/region/RegionGrabber.h
        src/widgets/ConfigWidget.cpp src/widgets/ConfigWidget.h
        src/widgets/MainWindow.cpp src/widgets/MainWindow.h
        src/widgets/PinnedArea.cpp src/widgets/PinnedArea.h
        src/Config.cpp src/Config.h
        src/Screenshot.cpp src/Screenshot.h
        src/Uploader.cpp src/Uploader.h
        src/Utils.h
        src/FileUtils.h
        src/json.hpp
        src/widgets/ListenableLineEdit.cpp src/widgets/ListenableLineEdit.cpp src/widgets/ListenableLineEdit.h)
install(TARGETS tigercapture DESTINATION ${CMAKE_INSTALL_PREFIX})

target_link_libraries(tigercapture Qt5::Core Qt5::Widgets curl)

#include_directories("/usr/x86_64-w64-mingw32/include/")
