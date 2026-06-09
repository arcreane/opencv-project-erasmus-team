cmake_minimum_required(VERSION 3.16)

project(imageEditorErasmus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

if(NOT CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH 
        "C:/Qt/6.11.0/msvc2022_64" 
        "C:/Users/Dell/Downloads/opencv/build"
    )
endif()

if(NOT OpenCV_DIR)
    set(OpenCV_DIR "C:/Users/Dell/Downloads/opencv/build")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(OpenCV REQUIRED)

set(TS_FILES imageEditorErasmus_fr_FR.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/EditorModel.cpp
        src/UndoRedoManager.cpp
        include/MainWindow.hpp
        include/EditorModel.hpp
        include/Command.hpp
        include/ThresholdCommand.hpp
        include/CLAHECommand.hpp
        include/MorphologyCommand.hpp
        include/RotateCommand.hpp
        include/AffineCommand.hpp
        include/PerspectiveCommand.hpp
        src/mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(imageEditorErasmus
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(imageEditorErasmus ${PROJECT_SOURCES})
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(imageEditorErasmus PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    ${OpenCV_LIBS})

target_include_directories(imageEditorErasmus PRIVATE 
    include 
    ${OpenCV_INCLUDE_DIRS}       
)

if(WIN32_EXECUTABLE)
    set_target_properties(imageEditorErasmus PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(imageEditorErasmus)
endif()

if(WIN32)
    get_filename_component(QT_BIN_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin" ABSOLUTE)

    add_custom_command(TARGET imageEditorErasmus PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:imageEditorErasmus>/platforms"
        COMMAND ${CMAKE_COMMAND} -E remove -f "$<TARGET_FILE_DIR:imageEditorErasmus>/Qt6*.dll"
    )

    add_custom_command(TARGET imageEditorErasmus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "C:/Users/Dell/Downloads/opencv/build/x64/vc16/bin/opencv_world${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}$<IF:$<CONFIG:Debug>,d,>.dll"
        "$<TARGET_FILE_DIR:imageEditorErasmus>"
    )

    add_custom_command(TARGET imageEditorErasmus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${QT_BIN_DIR}/Qt6Core.dll"
        "${QT_BIN_DIR}/Qt6Gui.dll"
        "${QT_BIN_DIR}/Qt6Widgets.dll"
        "$<TARGET_FILE_DIR:imageEditorErasmus>"
    )
    
    add_custom_command(TARGET imageEditorErasmus POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" --dir "$<TARGET_FILE_DIR:imageEditorErasmus>" "$<TARGET_FILE:imageEditorErasmus>"
    )
endif()