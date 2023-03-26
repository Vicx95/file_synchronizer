cmake_minimum_required(VERSION 3.20)
include(FetchContent)

# --- Fetch FTXUI --------------------------------------------------------------
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
# --- Fetch googletest ---------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# ------------------------------------------------------------------------------

FetchContent_MakeAvailable(ftxui googletest)

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
    src/view_ftx.cpp
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

# ------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ftxui::screen 
    ftxui::dom 
    ftxui::component
    )

# ------------------------------------------------------------------------------

add_library(${PROJECT_NAME}-lib ${SOURCES})
target_include_directories(${PROJECT_NAME}-lib 
  PUBLIC
    inc
  ${ftxui_SOURCE_DIR}/include
  )

# ------------------------------------------------------------------------------

enable_testing()
add_subdirectory(tests)
