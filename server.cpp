#include <iostream>
#include "SharedTable.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>



using namespace std;

template<class T>
void printPacket(T *table, size_t length) {
    cout << "Packet: [";
    for (size_t i = 0; i < length; ++i)
        cout << table[i] << ", ";
    cout << "] \n";
}

int main() {

    SharedTable<int> sharedTable("testMemory",10,boost::interprocess::read_write);
    int table[10];

    while (true) {
        sharedTable.readData(table);

        printPacket(table,10);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        if (table[0] == 10) break;
    }

    return 0;
}