//
// Created by geox on 28.07.15.
//

#ifndef CLIENT_SHAREDMEMORY_H
#define CLIENT_SHAREDMEMORY_H


//#define POSIX__SHARED__MEMORY


#include <cstring>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#if defined(__linux__) || defined(POSIX__SHARED__MEMORY) // linux
    #include <boost/interprocess/shared_memory_object.hpp>
#else // windows
    #include <boost/interprocess/windows_shared_memory.hpp>
#endif



class SharedMemory {
public:

    SharedMemory(char const *name, size_t size, boost::interprocess::mode_t mode);
    SharedMemory(char const *name, boost::interprocess::mode_t mode);
    ~SharedMemory();

    inline size_t size() const { return mappedRegion_.get_size(); }

    inline void readData(void *table, size_t size) {
        mtx_.lock();
        std::memcpy(table,mappedRegion_.get_address(),size);
        mtx_.unlock();
    }

    inline void writeData(void *table, size_t size) {
        mtx_.lock();
        std::memcpy(mappedRegion_.get_address(),table,size);
        mtx_.unlock();
    }

private:

    void operator=(SharedMemory const &) {}

protected:

#if defined(__linux__) || defined(POSIX__SHARED__MEMORY)  // linux
    boost::interprocess::shared_memory_object sharedMemory_;
    bool createOnly_;
#else // windows
    boost::interprocess::windows_shared_memory sharedMemory_;
#endif

    boost::interprocess::named_mutex mtx_;
    boost::interprocess::mapped_region mappedRegion_;

};


#endif //CLIENT_SHAREDMEMORY_H
