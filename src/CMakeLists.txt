set(SOURCES 
    controller.cpp
    main.cpp
    model.cpp
    view.cpp
    view_ftx.cpp
    view_console.cpp
    timer.cpp
    file_synchronizer.cpp
    scanner.cpp
    serializer.cpp
    logger.cpp
    #thread_pool.cpp
    )

set(HEADERS 
    inc/thread_pool.hpp
    )

add_library(${PROJECT_NAME}-lib STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME}-lib 
  PUBLIC
    inc
    ${ftxui_SOURCE_DIR}/include
  )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
      ftxui::screen 
      ftxui::dom 
      ftxui::component
      )
  