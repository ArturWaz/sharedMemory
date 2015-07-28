//
// Created by geox on 28.07.15.
//




#include <SharedTable.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>



template<class T>
void tableSquare(T *table, size_t length) {
    for (size_t i = 0; i < length; ++i)
        table[i] = table[i]*table[i];
}

template<class T>
void tableMultiply2x(T *table, size_t length) {
    for (size_t i = 0; i < length; ++i)
        table[i] *= 2;
}


int main() {

    SharedTable<int> sharedTable("testMemory",boost::interprocess::read_write);

    int table[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (size_t i = 0; i < 10; ++i) {

        sharedTable.writeData(table);

        tableMultiply2x(table,10);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }

    table[0] = 10;
    sharedTable.writeData(table);

    return 0;
}