cmake_minimum_required(VERSION 3.19)
project(cube-timer LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

# Main Target ================================================================================
if (APPLE)
	set(MACOSX_BUNDLE_ICON_FILE icon.icns)
	set(APP_ICON_MACOSX icon.icns)
	set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	add_executable(${PROJECT_NAME} MACOSX_BUNDLE src/main.cpp files.qrc ${APP_ICON_MACOSX} src/MainWindow.cpp src/MainWindow.hpp)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace NSHighResolutionCapable -bool true ${PROJECT_NAME}.app/Contents/Info.plist)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace CFBundleVersion -string ${PROJECT_VERSION} ${PROJECT_NAME}.app/Contents/Info.plist)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace CFBundleName -string ${PROJECT_NAME} ${PROJECT_NAME}.app/Contents/Info.plist)
else()
	add_executable(${PROJECT_NAME} src/main.cpp files.qrc src/MainWindow.cpp src/MainWindow.hpp)
endif()
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

# Tests ==========================================================================================
add_executable(test_divers test/test_divers.cpp)