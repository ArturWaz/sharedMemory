
set(sharedMemory_source_files SharedMemory.cpp SharedTable.h SharedMemory.h)

add_library(sharedMemory ${sharedMemory_source_files})

if (UNIX)
    target_link_libraries(sharedMemory pthread rt boost_system boost_thread)
elseif (WIN32)
    target_link_libraries(sharedMemory pthread boost_system-mt boost_thread-mt)
else ()
    message( FATAL_ERROR "\nSystem is not supported, cannot compile program." )
endif ()