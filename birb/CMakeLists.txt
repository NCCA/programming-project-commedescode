cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
project(BirbBuild VERSION 0.1 LANGUAGES CXX)
set(TargetName Birb)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(NGL CONFIG REQUIRED)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

add_executable(${TargetName})
target_sources(${TargetName} PRIVATE
    src/main.cpp
    src/MainWindow.cpp
    src/NGLScene.cpp
    src/flock.cpp
    src/NGLSceneMouseControls.cpp
    include/MainWindow.h
    include/flock.h
    include/NGLScene.h
    include/WindowParams.h
    ui/MainWindow.ui
    shaders/birbFragment.glsl
    shaders/birbVertex.glsl
)
target_include_directories(${TargetName} PRIVATE include)
target_link_libraries(${TargetName} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(${TargetName} PRIVATE NGL)

add_custom_target(${TargetName}CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${TargetName}>/shaders
)





