//
// Created by geox on 28.07.15.
//

#include "SharedMemory.h"



#if defined(__linux__) || defined(POSIX__SHARED__MEMORY) // linux

SharedMemory::SharedMemory(char const *name, size_t size, boost::interprocess::mode_t mode)
        : sharedMemory_(boost::interprocess::create_only,name,mode),
          createOnly_(true),
          mtx_(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()) {
    sharedMemory_.truncate(size);
    mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
}

SharedMemory::SharedMemory(char const *name, boost::interprocess::mode_t mode)
        : sharedMemory_(boost::interprocess::open_only,name,mode),
          createOnly_(false),
          mtx_(boost::interprocess::open_only,(std::string(name)+std::string("_mtx")).c_str()) {
    mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
}

SharedMemory::~SharedMemory() {
    if (createOnly_) {
        boost::interprocess::named_mutex::remove((std::string(sharedMemory_.get_name())+std::string("_mtx")).c_str());
        boost::interprocess::shared_memory_object::remove(sharedMemory_.get_name());
    }
}

#else // windows

SharedMemory::SharedMemory(char const *name, size_t size, boost::interprocess::mode_t mode)
            : sharedMemory_(boost::interprocess::create_only,name,mode,size),
              mtx_(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()),
              mappedRegion_(sharedMemory_,mode) {}

SharedMemory::SharedMemory(char const *name, boost::interprocess::mode_t mode)
            : sharedMemory_(boost::interprocess::open_only,name,mode),
              mtx_(boost::interprocess::open_only,(std::string(name)+std::string("_mtx")).c_str()),
              mappedRegion_(sharedMemory_,mode) {}

SharedMemory::~SharedMemory() {}

#endif