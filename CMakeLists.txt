cmake_minimum_required(VERSION 2.8.10)



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
if ( UNIX )
    set( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
endif ( UNIX )



include_directories(sharedMemory)

set(libraries pthread rt boost_system boost_thread)


set(PROJECT_NAME server)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries (${PROJECT_NAME} ${libraries})



set(PROJECT_NAME client)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries (${PROJECT_NAME} ${libraries})