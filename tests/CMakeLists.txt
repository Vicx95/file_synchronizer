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

add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-ut)