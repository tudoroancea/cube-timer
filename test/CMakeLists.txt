set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS -pthread)

# include GoogleTest ==========================================
include(FetchContent)
FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if (WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

include_directories(../src)
include(GoogleTest)

# executables ================================================
add_executable(test_divers test_divers.cpp)
target_link_libraries(test_divers ${REQUIRED_LIBS_QUALIFIED} timer)

add_executable(durationUnitTest durationUnitTest.cpp)
target_link_libraries(durationUnitTest PUBLIC gtest gtest_main timer)
gtest_discover_tests(durationUnitTest)

add_executable(scrambleUnitTest scrambleUnitTest.cpp)
target_link_libraries(scrambleUnitTest PUBLIC gtest gtest_main timer)
gtest_discover_tests(scrambleUnitTest)