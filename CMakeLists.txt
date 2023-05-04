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

find_package(Boost 1.74.0 REQUIRED system)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

set(SOURCES 
    src/controller.cpp
    src/main.cpp
    src/model.cpp
    src/view.cpp
    src/timer.cpp
    src/file_synchronizer.cpp
    src/scanner.cpp
    src/serializer.cpp
    src/config_manager.cpp
    src/file_streamer.cpp
    src/logger.cpp
    #src/thread_pool.cpp
    )

set(HEADERS 
    inc/thread_pool.hpp
    )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
set(FLAGS -Wall -Wextra -Werror -Wconversion -Wpedantic -g)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

FetchContent_MakeAvailable(googletest)

enable_testing()
set(UT-SOURCES 
    tests/model_ut.cpp
    )

add_library(${PROJECT_NAME}-lib STATIC ${SOURCES})
add_executable(${PROJECT_NAME}-ut ${UT-SOURCES})
link_libraries(gmock)
target_link_libraries(${PROJECT_NAME}-ut gtest_main gmock ${PROJECT_NAME}-lib)
target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})
add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-ut)
