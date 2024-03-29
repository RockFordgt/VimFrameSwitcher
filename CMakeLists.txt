cmake_minimum_required(VERSION 3.10)

project(VimFrameSwitcher)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QtCreator REQUIRED COMPONENTS Core)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
set(QtX Qt${QT_VERSION_MAJOR})

add_qtc_plugin(VimFrameSwitcher
  PLUGIN_DEPENDS
    QtCreator::Core
    QtCreator::TextEditor
  DEPENDS
    ${QtX}::Widgets
    QtCreator::ExtensionSystem
    QtCreator::Utils
  SOURCES
    .github/workflows/build_cmake.yml
    .github/workflows/README.md
    README.md
    vimframeswitcher.cpp
    vimframeswitcher.hpp
    vimframeswitcher_global.hpp
    vimframeswitcherconstants.hpp
)
