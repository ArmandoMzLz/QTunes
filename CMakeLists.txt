cmake_minimum_required(VERSION 3.16)

project(QTunes VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appQTunes
    main.cpp
)

qt_add_qml_module(appQTunes
    URI QTunes
    QML_FILES
        Main.qml
    SOURCES
        mediaplayer.h
        mediaplayer.cpp


        SOURCES getfolder.h getfolder.cpp
        SOURCES sqlconnection.h sqlconnection.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQTunes PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQTunes
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQTunes
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appQTunes
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql Multimedia)
target_link_libraries(appQTunes
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Multimedia
)
