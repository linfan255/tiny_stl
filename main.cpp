#include <iostream>
#include <vector>
#include <list>
#include "vector.h"
#include "allocator.h"
#include "lf_uninitialized.h"
#include "lf_algorithm.h"
#include "lf_list.h"

using namespace std;

void print(const lf::vector<int> vec) {
    for(size_t i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}

int main() {
    lf::list<int> lst1;
    lf::list<int> lst2;

    lst1.push_back(2);
    lst1.push_back(5);
    lst1.push_back(7);
    lst1.push_back(8);

    lst2.push_back(4);
    lst2.push_back(6);
    lst2.push_back(11);
    lst2.push_back(20);
    lst2.push_back(23);

    lst1.merge(lst2);
    lst1.reverse();
    for(auto it = lst1.begin(); it != lst1.end(); it++)
        cout << *it << " ";
    cout << endl;

    return 0;
}