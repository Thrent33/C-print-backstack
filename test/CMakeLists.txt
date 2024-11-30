set(BIN_NAME ${PROJECT_NAME}_TEST)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${BIN_NAME} ${SRC})
target_link_libraries(${BIN_NAME} ${PROJECT_NAME} gtest gmock cpp-backstack)
