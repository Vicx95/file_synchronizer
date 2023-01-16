cmake_minimum_required(VERSION 3.20)
project(file_synchronizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

find_package(nlohmann_json 3.2.0 REQUIRED)

# --- Fetch FTXUI --------------------------------------------------------------
 set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Important: Specify a GIT_TAG XXXXX here.
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
 
# ------------------------------------------------------------------------------
 
#project(ftxui-starter
#  LANGUAGES CXX
#  VERSION 1.0.0
#)
 
#add_executable(ftxui-starter src/main.cpp)
 
#target_link_libraries(${PROJECT_NAME}
#  PRIVATE ftxui::screen
#  PRIVATE ftxui::dom
#  PRIVATE ftxui::component # Not needed for this example.
#)

set(SOURCES 
    src/controller.cpp
    src/main.cpp
    src/model.cpp
    src/view.cpp
    src/timer.cpp
    src/FileSynchronizer.cpp 
    src/Scanner.cpp
    src/Serializer.cpp
    src/UserInterface.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})
set(FLAGS -Wall -Wextra -Werror -Wconversion -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_include_directories(file_synchronizer PRIVATE src)

FetchContent_MakeAvailable(googletest)

enable_testing()
set(UT-SOURCES 
    tests/main.cpp
    )

add_executable(${PROJECT_NAME}-ut ${UT-SOURCES})
link_libraries(gmock)
target_link_libraries(${PROJECT_NAME}-ut gtest_main gmock nlohmann_json::nlohmann_json) # ftxui::screen ftxui::dom ftxui::component)
target_link_libraries(${PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)
target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})
add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-ut)
