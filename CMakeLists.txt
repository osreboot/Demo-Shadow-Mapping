cmake_minimum_required(VERSION 3.23)
project(DemoShadowMapping)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp)

# Disable the console window if this is a release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
endif()

# Copy resources folder to build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/)

# Allow #includes to reference these directories
include_directories(lib)
include_directories(src)

# Add src CMakeLists.txt
add_subdirectory(src)

# Add library CMakeLists.txt
add_subdirectory(lib/glfw)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew)

target_link_libraries(${PROJECT_NAME} glfw libglew_static -static)