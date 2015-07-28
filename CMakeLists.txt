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

if ( UNIX )
    set(libraries pthread rt boost_system boost_thread)
elseif ( WIN32 )
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} C:/msys64/mingw64/lib)
    set(libraries pthread boost_system-mt boost_thread-mt)
else ()
    message( FATAL_ERROR "\nSystem is not supported, cannot compile program." )
endif ()


set(PROJECT_NAME server)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries (${PROJECT_NAME} ${libraries})



set(PROJECT_NAME client)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries (${PROJECT_NAME} ${libraries})
