cmake_minimum_required(VERSION 3.14)
project(final)#TODO change this name to something cooler
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp Engine.cpp Engine.hpp Player.cpp Player.hpp Obstacle.hpp Obstacle.cpp CustomObjects.cpp CustomObjects.hpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Windows with MinGW Installations
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" AND MINGW )
    # if working on Windows but not in the lab
    # update the include directory location
    include_directories("Z:/CSCI441/include")
    # update the lib directory location
    target_link_directories(${PROJECT_NAME} PUBLIC "Z:/CSCI441/lib")
    target_link_directories(${PROJECT_NAME} PUBLIC "./lib/")
    target_link_directories(${PROJECT_NAME} PUBLIC "./include")
    target_link_libraries(${PROJECT_NAME} opengl32 glfw3 glew32.dll gdi32 stbimage)
# OS X Installations
elseif( APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    # update the include directory location
    include_directories("/usr/local/include")
    # update the lib directory location
    target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/lib")
    target_link_directories(${PROJECT_NAME} PUBLIC "./include")
    target_link_directories(${PROJECT_NAME} PUBLIC "./lib/")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" GLFW glew stbimage)
# Blanket *nix Installations
elseif( UNIX AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    include_directories("/usr/local/include")
    include_directories("./include/")
    # update the lib directory location
    target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/lib")
    target_link_directories(${PROJECT_NAME} PUBLIC "/usr/lib64")
    target_link_directories(${PROJECT_NAME} PUBLIC "./lib/")

		target_link_libraries(${PROJECT_NAME} OpenGL glfw GLEW stbimage)

endif()
