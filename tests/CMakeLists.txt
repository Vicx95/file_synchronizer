include(FetchContent)

# --- Fetch googletest ---------------------------------------------------------
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
# ------------------------------------------------------------------------------

FetchContent_MakeAvailable(googletest)

set(UT-SOURCES 
    model_ut.cpp
    )

add_executable(${PROJECT_NAME}-ut ${UT-SOURCES})

target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})

target_link_libraries(${PROJECT_NAME}-ut 
    PRIVATE 
        gtest_main
        gmock
        ${PROJECT_NAME}-lib
        )

target_include_directories(${PROJECT_NAME}-ut
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/inc
        ${ftxui_SOURCE_DIR}/include
        )

add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-ut)