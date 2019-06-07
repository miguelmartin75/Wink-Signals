project(benchmark LANGUAGES CXX)

### Prepare sources
set(benchmark_HEADERS)
set(benchmark_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Benchmark.cpp
    )
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/"
    FILES
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        )

### Build executable
add_executable(${PROJECT_NAME} WIN32)
target_sources(${PROJECT_NAME} PRIVATE ${benchmark_SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        wink::signals)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCES_ROOT})

### Install
include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/wink/examples/"
    )

