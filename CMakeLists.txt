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
    inc/thread_pool_provider.hpp
    )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
set(FLAGS -Wall -Wextra -Werror -Wconversion -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

FetchContent_MakeAvailable(googletest)

enable_testing()
set(UT-SOURCES 
    tests/model_ut.cpp
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

add_library(${PROJECT_NAME}-lib STATIC ${SOURCES})
add_executable(${PROJECT_NAME}-ut ${UT-SOURCES})
link_libraries(gmock)
target_link_libraries(${PROJECT_NAME}-ut gtest_main gmock ${PROJECT_NAME}-lib)
target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})
add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-ut)
