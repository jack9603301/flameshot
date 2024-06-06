cmake_minimum_required(VERSION 3.14)

project(Watermark LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)

add_library(Watermark SHARED
  watermarkplugin.cpp
  watermarkplugin.h
  Watermark.json
  corepluginInterface.h
  corepluginInterface.cpp

)

target_link_libraries(Watermark PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

target_compile_definitions(Watermark PRIVATE WATERMARK_LIBRARY)
