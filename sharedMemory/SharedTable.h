//
// Created by geox on 28.07.15.
//

#ifndef SHAREDMEMORY_SHAREDTABLE_H
#define SHAREDMEMORY_SHAREDTABLE_H


#include <stdexcept>
#include "SharedMemory.h"



template <class T>
class SharedTable: public SharedMemory {
public:

    inline SharedTable(char const *name, size_t size, boost::interprocess::mode_t mode):
            SharedMemory(name,size*sizeof(T),mode),
            size_(size) {}

    inline SharedTable(char const *name, boost::interprocess::mode_t mode):
            SharedMemory(name,mode),
            size_(SharedMemory::size()/sizeof(T)) {}

    ~SharedTable() {}


    inline size_t size() const { return size_; }
    inline size_t length() const { return size_; }


    inline void readData(T *table) {
        SharedMemory::readData(table,size()*sizeof(T));
    }

    inline void writeData(T *table) {
        SharedMemory::writeData(table,size()*sizeof(T));
    }


    inline void readData(T *table, size_t tableLength) {
        SharedMemory::readData(table,tableLength*sizeof(T));
    }

    inline void writeData(T *table, size_t tableLength) {
        SharedMemory::writeData(table,tableLength*sizeof(T));
    }


private:

    void operator=(SharedTable const &) {}
    inline T &get_(size_t i) { return (static_cast<T*>(mappedRegion_.get_address()))[i]; }

    size_t size_;

};


#endif //SHAREDMEMORY_SHAREDTABLE_H
