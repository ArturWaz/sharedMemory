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

    cout << "Memory size [ SharedTable::size() ]:     " << sharedTable.size() << endl;
    cout << "Memory size [ SharedMemory::size() ]:    " << sharedTable.SharedMemory::size() << endl;

    while (true) {
        sharedTable.readData(table,10);

        printPacket(table,10);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        if (table[0] == 10) break;
    }

    return 0;
}