cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(wink LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQURED ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

###
# Project version
###
set(wink_VERSION_MAJOR 0)
set(wink_VERSION_MINOR 1)
set(wink_VERSION_PATCH 0)
set(wink_VERSION "${wink_VERSION_MAJOR}.${wink_VERSION_MINOR}.${wink_VERSION_PATCH}")
message(STATUS "wink version: ${wink_VERSION}")

###
# Build wink-signals header-only library
###
set(HEADERS
    ${CMAKE_SOURCE_DIR}/wink/event_queue.hpp
    ${CMAKE_SOURCE_DIR}/wink/signal.hpp
    ${CMAKE_SOURCE_DIR}/wink/slot.hpp
    # detail
    ${CMAKE_SOURCE_DIR}/wink/detail/FastDelegate.h
    ${CMAKE_SOURCE_DIR}/wink/detail/FastDelegateBind.h
    )
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS})


add_library(signals INTERFACE)
add_library(wink::signals ALIAS signals)

target_compile_definitions(signals INTERFACE
    LIBRARY_HEADER_ONLY
    )
#target_sources(signals INTERFACE $<BUILD_INTERFACE:${HEADERS}>) # doesnt work this way
target_include_directories(signals INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    )

# hack to show interface target's headers within IDEs like Xcode
add_custom_target(signals-header-only SOURCES ${HEADERS})

###
# Install
###
install(
    TARGETS signals
    EXPORT wink-targets
    )
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/wink
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

###
# Build/install examples
###
set(wink_BUILD_EXAMPLES ON CACHE BOOL "Build examples")

if(wink_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(wink_BUILD_EXAMPLES)

###
# Handle cmake config
###
# set cmake config paths
set(wink_VERSION_CONFIG ${PROJECT_BINARY_DIR}/wink-config-version.cmake)
set(wink_PROJECT_CONFIG ${PROJECT_BINARY_DIR}/wink-config.cmake)
set(wink_EXPORT_NAME "wink-targets")
set(wink_TARGETS_CONFIG ${PROJECT_BINARY_DIR}/wink-targets.cmake)
set(wink_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}/)
# export cmake config
write_basic_package_version_file(${wink_VERSION_CONFIG}
                                 VERSION ${wink_VERSION}
                                 COMPATIBILITY AnyNewerVersion
                                 )
configure_package_config_file(${PROJECT_SOURCE_DIR}/wink-config.cmake.in
                              ${wink_PROJECT_CONFIG}
                              INSTALL_DESTINATION ${wink_CONFIG_INSTALL_DIR}
                              )
export(EXPORT wink-targets
       FILE ${wink_TARGETS_CONFIG}
       NAMESPACE wink::
       )
# install cmake config
install(FILES ${wink_PROJECT_CONFIG} ${wink_VERSION_CONFIG}
        DESTINATION ${wink_CONFIG_INSTALL_DIR}
        )
install(EXPORT wink-targets
        DESTINATION ${wink_CONFIG_INSTALL_DIR}
        NAMESPACE wink::
        )
# install misc files
#install(DIRECTORY cmake
        #DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}/
        #)
install(FILES README.md LICENSE
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}/
        )
