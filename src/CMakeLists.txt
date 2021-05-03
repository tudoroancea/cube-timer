set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Timer library ===========================================================================
add_library(timer STATIC
			MainWindow.cpp MainWindow.hpp
			Duration.cpp Duration.hpp
			Scramble.cpp Scramble.hpp
			MainWindow2.cpp MainWindow2.hpp
			TimesList.cpp TimesList.hpp
			rapidcsv.h
			)
target_link_libraries(timer ${REQUIRED_LIBS_QUALIFIED})
# Main Target ================================================================================
if (APPLE)
#	Set icon file paths
	set(MACOSX_BUNDLE_ICON_FILE icon.icns)
	set(APP_ICON_MACOSX icon.icns)
	set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
#	Set other Resources
	set(RESOURCES_FILES
		test.csv)
#	Create the executable and links the libraries
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE main.cpp files.qrc icon.icns test.csv)
	target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} timer)
#	Set various bundle properties
	set_target_properties(${PROJECT_NAME} PROPERTIES
						  MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
						  MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
						  MACOSX_BUNDLE_COPYRIGHT "Copyright © Tudor Oancea. Tous droits réservés."
						  MACOSX_BUNDLE_GUI_IDENTIFIER "com.tudor.oancea.${PROJECT_NAME}"
						  MACOSX_BUNDLE_ICON_FILE "icon.icns"
						  RESOURCE "${RESOURCES_FILES}")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND plutil -replace NSHighResolutionCapable -bool true ${PROJECT_NAME}.app/Contents/Info.plist)
else()
	message(FATAL_ERROR "The application must be built on macOS")
endif()