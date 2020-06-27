#include <iostream>
#include "rbtree.h"

using namespace std;

int main()
{
        rb_tree<int> tree;
        int data[] = { 12, 1, 9, 2, 0, 11, 7, 19, 4, 15, 18, 5, 14, 13, 10, 16, 6, 3, 8, 17 };
        int max_len = sizeof(data) / sizeof(int);
        cout << "test_rb_tree: insert" << endl;
        for (auto i = 0; i < max_len; i++)
        {
            auto debug = data[i];
            tree.insert(debug);
            cout << "insert " << debug << endl;
        }

        cout << "test_rb_tree: dump" << endl;
        tree.dump();
        cout << "test_rb_tree: height" << endl;
        cout << "height: " << tree.height() << endl;

        for (auto i = 0; i < max_len; i++)
        {
            auto debug = data[i];
            tree.erase(debug);
            cout << "test_rb_tree: height: " << tree.height() << " erase: " << debug << endl;
            cout << "test_rb_tree: dump" << endl;
            tree.check_rb_feature_5();
            tree.dump();
        }

        return 0;
}
