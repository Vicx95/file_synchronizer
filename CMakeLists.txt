cmake_minimum_required(VERSION 3.20)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# ------------------------------------------------------------------------------

project(file_synchronizer
  LANGUAGES CXX
  VERSION 1.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
    src/controller.cpp
    src/main.cpp
    src/model.cpp
    src/view.cpp
    src/timer.cpp
    src/file_synchronizer.cpp
    src/scanner.cpp
    src/serializer.cpp
    src/logger.cpp
    #src/thread_pool.cpp
    )

set(HEADERS 
    inc/thread_pool.hpp
    )

set(FLAGS -Wall -Wextra -Werror -Wconversion -Wpedantic -g)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_include_directories(file_synchronizer PRIVATE src)
target_link_libraries(${PROJECT_NAME} ftxui::screen ftxui::dom ftxui::component)

FetchContent_MakeAvailable(googletest)

enable_testing()
set(UT-SOURCES 
    tests/model_ut.cpp
    )

#add_library(${PROJECT_NAME}-lib STATIC ${SOURCES})
add_executable(${PROJECT_NAME}-ut ${UT-SOURCES})
link_libraries(gmock)
target_link_libraries(${PROJECT_NAME}-ut gtest_main gmock ${PROJECT_NAME}-lib)

target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})
add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-ut)
