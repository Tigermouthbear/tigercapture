cmake_minimum_required(VERSION 3.17)
project(tigercapture)

set(CMAKE_CXX_STANDARD 14)

# Tell cmake where Qt is located
set(Qt5_DIR "/usr/lib/cmake/Qt5/")

# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(tigercapture src/main.cpp src/MainWindow.cpp src/MainWindow.h src/Screenshot.cpp src/Screenshot.h src/region/RegionGrabber.cpp src/region/RegionGrabber.h src/region/AreaScreenshotGrabber.cpp src/region/AreaScreenshotGrabber.h src/region/PinnedAreaGrabber.cpp src/region/PinnedAreaGrabber.h src/region/PinnedArea.cpp src/region/PinnedArea.h src/FileUtils.h src/Utils.h)

# Link the library to the executable
target_link_libraries(tigercapture Qt5::Core Qt5::Widgets)