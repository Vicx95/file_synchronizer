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
FetchContent_MakeAvailable(ftxui)

project(file_synchronizer
  LANGUAGES CXX
  VERSION 1.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FLAGS -Wall -Wextra -Werror -Wconversion -Wpedantic -g)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)