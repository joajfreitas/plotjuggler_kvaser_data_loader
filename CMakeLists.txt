cmake_minimum_required(VERSION 3.10)

project(plotjuggler_sample_plugins)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#-------  Include Qt dependencies -------
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

SET(CMAKE_CXX_FLAGS -O3)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Concurrent
    Xml
    Svg
    OpenGL)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Xml_INCLUDE_DIRS}
    ${Qt5Svg_INCLUDE_DIRS} )

set(QT_LIBRARIES
    Qt5::Core
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg )

add_definitions( ${QT_DEFINITIONS} -DQT_PLUGIN )
set( PJ_LIBRARIES ${QT_LIBRARIES} )

#--------------------------------------------------------

find_package(plotjuggler REQUIRED)
message(STATUS "PlotJuggler FOUND")
message(STATUS "plotjuggler_INCLUDE_DIR: ${plotjuggler_INCLUDE_DIR}")
message(STATUS "plotjuggler_LIBRARIES: ${plotjuggler_LIBRARIES}")

include_directories(${plotjuggler_INCLUDE_DIR})
list(APPEND PJ_LIBRARIES ${plotjuggler_LIBRARIES} )
set(PJ_PLUGIN_INSTALL_DIRECTORY bin )

#------- Create the libraries -------

# todo: write CMakeLists.txt for fcp-cpp
add_library(PlotJugglerDataSample SHARED
    kvaser_data_load/kvaser_data_load.h
    kvaser_data_load/kvaser_data_load.cpp
    fcp-cpp/src/candata.h
    fcp-cpp/src/fcp_argument.cpp
    fcp-cpp/src/fcp_argument.hpp
    fcp-cpp/src/fcp_command.cpp
    fcp-cpp/src/fcp_command.hpp
    fcp-cpp/src/fcp_common.hpp
    fcp-cpp/src/fcp_config.cpp
    fcp-cpp/src/fcp_config.hpp
    fcp-cpp/src/fcp.cpp
    fcp-cpp/src/fcp_device.cpp
    fcp-cpp/src/fcp_device.hpp
    fcp-cpp/src/fcp.hpp
    fcp-cpp/src/fcp_log.cpp
    fcp-cpp/src/fcp_log.hpp
    fcp-cpp/src/fcp_message.cpp
    fcp-cpp/src/fcp_message.hpp
    fcp-cpp/src/fcp_signal.cpp
    fcp-cpp/src/fcp_signal.hpp
    fcp-cpp/src/json.hpp
    fcp-cpp/src/signal_parser.c
    fcp-cpp/src/signal_parser.h
   )

target_link_libraries(PlotJugglerDataSample
    ${PJ_LIBRARIES})

#------- Install the libraries -------

install(
    TARGETS
        PlotJugglerDataSample
    DESTINATION
        ${PJ_PLUGIN_INSTALL_DIRECTORY}  )

