set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Timer library ===========================================================================
add_library(timer STATIC
			MainWindow.cpp MainWindow.hpp
			Duration.cpp Duration.hpp
			Scramble.cpp Scramble.hpp)
target_link_libraries(timer ${REQUIRED_LIBS_QUALIFIED})
# Main Target ================================================================================
if (APPLE)
	set(MACOSX_BUNDLE_ICON_FILE icon.icns)
	set(APP_ICON_MACOSX icon.icns)
	set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	add_executable(${PROJECT_NAME} MACOSX_BUNDLE main.cpp files.qrc ${APP_ICON_MACOSX})

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace NSHighResolutionCapable -bool true ${PROJECT_NAME}.app/Contents/Info.plist)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace CFBundleVersion -string ${PROJECT_VERSION} ${PROJECT_NAME}.app/Contents/Info.plist)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace CFBundleName -string ${PROJECT_NAME} ${PROJECT_NAME}.app/Contents/Info.plist)
else()
	add_executable(${PROJECT_NAME} main.cpp files.qrc)
endif()
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} timer)