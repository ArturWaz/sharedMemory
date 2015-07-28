//
// Created by geox on 28.07.15.
//

#ifndef SHAREDMEMORY_SHAREDTABLE_H
#define SHAREDMEMORY_SHAREDTABLE_H


#include <vector>
#include <stdexcept>
#include <cstring>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

//#ifdef __linux__ // linux
    #include <boost/interprocess/shared_memory_object.hpp>
/*#else // windows
    #include <boost/interprocess/windows_shared_memory.hpp>
#endif*/


template <class T>
class SharedTable {
    //static_assert(std::is_arithmetic<T>::value, "\nOnly basic types are supported.\n");
public:

//#ifdef __linux__ // linux

    SharedTable(char const *name, size_t size, boost::interprocess::mode_t mode)
                : sharedMemory_(boost::interprocess::create_only,name,mode),
                  createOnly_(true),
                  mtx_(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()) {
        sharedMemory_.truncate(size*sizeof(T));
        mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
    }

    SharedTable(char const *name, boost::interprocess::mode_t mode)
                : sharedMemory_(boost::interprocess::open_only,name,mode),
                  createOnly_(false),
                  mtx_(boost::interprocess::open_only,(std::string(name)+std::string("_mtx")).c_str()) {
        mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
    }

    ~SharedTable() {
        if (createOnly_) {
            boost::interprocess::named_mutex::remove((std::string(sharedMemory_.get_name())+std::string("_mtx")).c_str());
            boost::interprocess::shared_memory_object::remove(sharedMemory_.get_name());
        }
    }

/*#else // windows

    SharedTable(char const *name, size_t size, boost::interprocess::mode_t mode)
                : sharedMemory_(boost::interprocess::create_only,name,mode,size*sizeof(T)),
                  mtx_(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()),
                  mappedRegion_(sharedMemory_,mode) {}

    SharedTable(char const *name, boost::interprocess::mode_t mode)
                : sharedMemory_(boost::interprocess::open_only,name,mode),
                  mtx_(boost::interprocess::open_only,(std::string(name)+std::string("_mtx")).c_str()),
                  mappedRegion_(sharedMemory_,mode) {}

    ~SharedTable() {}

#endif*/



    inline size_t size() const { return mappedRegion_.get_size()/sizeof(T); }
    inline size_t length() const { return mappedRegion_.get_size()/sizeof(T); }


    inline void readData(T *table) {
        mtx_.lock();
        std::memcpy(table,mappedRegion_.get_address(),mappedRegion_.get_size());
        mtx_.unlock();
    }

    inline void writeData(T *table) {
        mtx_.lock();
        std::memcpy(mappedRegion_.get_address(),table,mappedRegion_.get_size());
        mtx_.unlock();
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



//#ifdef __linux__ // linux

    boost::interprocess::shared_memory_object sharedMemory_;
    bool createOnly_;

/*#else // windows

    boost::interprocess::windows_shared_memory sharedMemory_;

#endif*/

    boost::interprocess::named_mutex mtx_;
    boost::interprocess::mapped_region mappedRegion_;

};


#endif //SHAREDMEMORY_SHAREDTABLE_H
