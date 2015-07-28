//
// Created by geox on 28.07.15.
//

#ifndef SHAREDMEMORY_SHAREDTABLE_H
#define SHAREDMEMORY_SHAREDTABLE_H


#include <vector>
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


    void readData(std::vector<T> &v) {
        if (v.size() != size()) throw std::length_error("Sizes of shared table and std::vector are different, function: SharedTable::getTable(std::vector<T> &)");
        mtx_.lock();
        for (int i = 0; i < v.size(); ++i) {
            v[i] = get_(i);
        }
        mtx_.unlock();
    }

    void writeData(std::vector<T> const &v) {
        if (v.size() != size()) throw std::length_error("Sizes of shared table and std::vector are different, function: SharedTable::setTable(std::vector<T> const &)");
        mtx_.lock();
        for (int i = 0; i < v.size(); ++i) {
            get_(i) = v[i];
        }
        mtx_.unlock();
    }


private:

    void operator=(SharedTable const &) {}
    inline T &get_(size_t i) { return (static_cast<T*>(mappedRegion_.get_address()))[i]; }

    size_t size_;

};


#endif //SHAREDMEMORY_SHAREDTABLE_H
