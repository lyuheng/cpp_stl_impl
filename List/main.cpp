#include <iostream>
#include "list.h"

using namespace std;

int main()
{
    list<int, int> ls;
    ls.push_back(1);
    ls.dump();
    ls.push_back(2);
    ls.dump();
    ls.push_back(3);
    ls.dump();

    ls.insert(4,1);
    ls.dump();

    ls.erase(1);
    ls.dump();

    ls.erase(1);
    ls.dump();

    ls.erase(1);
    ls.dump();

    ls.erase(0);
    ls.dump();
    return 0;
}
