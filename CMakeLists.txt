cmake_minimum_required(VERSION 3.17)
project(tigercapture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lcurl")

# Tell cmake where Qt is located
set(Qt5_DIR "/usr/lib/cmake/Qt5/")

# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# automoc!
set(CMAKE_AUTOMOC ON)

# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

# Add clip subdirectory to compile the library
add_subdirectory(clip)

add_executable(tigercapture src/main.cpp src/MainWindow.cpp src/MainWindow.h src/Screenshot.cpp src/Screenshot.h src/region/RegionGrabber.cpp src/region/RegionGrabber.h src/region/AreaScreenshotGrabber.cpp src/region/AreaScreenshotGrabber.h src/region/PinnedAreaGrabber.cpp src/region/PinnedAreaGrabber.h src/region/PinnedArea.cpp src/region/PinnedArea.h src/FileUtils.h src/Utils.h src/Uploader.cpp src/Uploader.h)

# Link the library to the executable
target_link_libraries(tigercapture Qt5::Core Qt5::Widgets curl clip)