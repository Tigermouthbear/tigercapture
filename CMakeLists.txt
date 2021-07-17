cmake_minimum_required(VERSION 3.12.4)
project(tigercapture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

set(CMAKE_INSTALL_PREFIX /usr/bin)

# Tell cmake where Qt is located
set(Qt5_DIR "/usr/lib/cmake/Qt5/")

# Tell cmake to find the modules Qt5Core, Qt5Widgets, and Qt5Network
find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

# automoc!
set(CMAKE_AUTOMOC ON)

# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

# Add clip subdirectory to compile the library
add_subdirectory(clip)

add_executable(tigercapture
        src/main.cpp
        src/json.hpp
        src/TigerCapture.hpp
        src/TigerCapture.cpp src/TigerCapture.h
        src/Config.cpp src/Config.h
        src/Screenshot.cpp src/Screenshot.h
        src/Uploader.cpp src/Uploader.h
        src/widgets/gui/MainWindow.cpp src/widgets/gui/MainWindow.h
        src/widgets/gui/ConfigWidget.cpp src/widgets/gui/ConfigWidget.h
        src/widgets/DragUploadWidget.cpp src/widgets/DragUploadWidget.h
        src/widgets/PinnedArea.cpp src/widgets/PinnedArea.h
        src/widgets/SystemTray.cpp src/widgets/SystemTray.h
        src/widgets/gui/UploadedFileWidget.cpp src/widgets/gui/UploadedFileWidget.h
        src/widgets/gui/UploadsExplorerWidget.cpp src/widgets/gui/UploadsExplorerWidget.h
        src/region/AreaScreenshotGrabber.cpp src/region/AreaScreenshotGrabber.h
        src/region/PinnedAreaGrabber.cpp src/region/PinnedAreaGrabber.h
        src/region/RegionGrabber.cpp src/region/RegionGrabber.h
        src/Singleton.cpp src/Singleton.h)

install(TARGETS tigercapture DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES data/tigercapture.desktop DESTINATION /usr/share/applications)
install(FILES data/icons/icon.ico data/icons/icon-large.ico DESTINATION /usr/share/tigercapture/icons)
install(FILES data/icons/hicolor/48x48/tigercapture.png DESTINATION /usr/share/icons/hicolor/48x48/apps)
install(FILES data/icons/hicolor/128x128/tigercapture.png DESTINATION /usr/share/icons/hicolor/128x128/apps)
install(FILES data/icons/hicolor/256x256/tigercapture.png DESTINATION /usr/share/icons/hicolor/256x256/apps)


target_link_libraries(tigercapture Qt5::Core Qt5::Widgets Qt5::Network clip)

target_compile_definitions(tigercapture PRIVATE INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

# .desktop and resources
configure_file(${CMAKE_SOURCE_DIR}/data/tigercapture.desktop ${CMAKE_CURRENT_BINARY_DIR}/share/applications/tigercapture.desktop COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/icons/icon.ico ${CMAKE_CURRENT_BINARY_DIR}/icons/icon.ico COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/icons/icon-large.ico ${CMAKE_CURRENT_BINARY_DIR}/icons/icon-large.ico COPYONLY)

# hicolor icons
configure_file(${CMAKE_SOURCE_DIR}/data/icons/hicolor/48x48/tigercapture.png ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/48x48/apps/tigercapture.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/icons/hicolor/128x128/tigercapture.png ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/128x128/apps/tigercapture.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/icons/hicolor/256x256/tigercapture.png ${CMAKE_CURRENT_BINARY_DIR}/share/icons/hicolor/256x256/apps/tigercapture.png COPYONLY)
