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

set(Qt6_DIR "C:/Qt/6.11.0/msvc2022_64/lib/cmake/Qt6")
set(Qt6Widgets_DIR "C:/Qt/6.11.0/msvc2022_64/lib/cmake/Qt6Widgets")
set(Qt6LinguistTools_DIR "C:/Qt/6.11.0/msvc2022_64/lib/cmake/Qt6LinguistTools")
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/msvc2022_64" "C:/Users/Dell/Downloads/opencv/build")
set(OpenCV_DIR "C:/Users/Dell/Downloads/opencv/build")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(OpenCV REQUIRED)

set(TS_FILES imageEditorErasmus_fr_FR.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/EditorModel.cpp
        include/MainWindow.hpp
        include/EditorModel.hpp
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
    if(ANDROID)
        add_library(imageEditorErasmus SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(imageEditorErasmus
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(imageEditorErasmus PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    ${OpenCV_LIBS})

target_include_directories(imageEditorErasmus PRIVATE 
    include 
    ${OpenCV_INCLUDE_DIRS}       
)

if(${QT_VERSION} VERSION_LESS "6.1.0")
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.imageEditorErasmus)
endif()

set_target_properties(imageEditorErasmus PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS imageEditorErasmus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(imageEditorErasmus)
endif()

if(WIN32)
    get_filename_component(QT_BIN_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin" ABSOLUTE)
    get_filename_component(QT_PLUGINS_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../../plugins" ABSOLUTE)

    add_custom_command(TARGET imageEditorErasmus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Users/Dell/Downloads/opencv/build/x64/vc16/bin/opencv_world4120$<IF:$<CONFIG:Debug>,d,>.dll"
        "$<TARGET_FILE_DIR:imageEditorErasmus>"
        COMMENT "Kopiowanie OpenCV DLL..."
    )

    add_custom_command(TARGET imageEditorErasmus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_BIN_DIR}/Qt${QT_VERSION_MAJOR}Core$<IF:$<CONFIG:Debug>,d,>.dll"
        "${QT_BIN_DIR}/Qt${QT_VERSION_MAJOR}Gui$<IF:$<CONFIG:Debug>,d,>.dll"
        "${QT_BIN_DIR}/Qt${QT_VERSION_MAJOR}Widgets$<IF:$<CONFIG:Debug>,d,>.dll"
        "$<TARGET_FILE_DIR:imageEditorErasmus>"
        COMMENT "Kopiowanie bibliotek DLL systemu Qt..."
    )

    add_custom_command(TARGET imageEditorErasmus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:imageEditorErasmus>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_PLUGINS_DIR}/platforms/qwindows$<IF:$<CONFIG:Debug>,d,>.dll"
        "$<TARGET_FILE_DIR:imageEditorErasmus>/platforms"
        COMMENT "Kopiowanie wtyczki graficznej platforms (qwindows)..."
    )
endif()