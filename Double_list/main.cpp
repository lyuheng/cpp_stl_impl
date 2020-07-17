#include <iostream>

#include "double_list.h"

using namespace std;

int main()
{
    double_list<int> list;
    list.dump();
    list.push_back(1);
    list.dump();
    list.push_back(2);
    list.dump();
    list.insert(3, 1);
    list.dump();
    list.insert(4, 0);
    list.dump();
    list.erase(0);
    list.dump();
    list.erase(3);
    list.dump();
    list.erase(1);
    list.dump();
    cout << list.len() << endl;
    return 0;
}
