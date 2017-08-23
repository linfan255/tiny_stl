#include <iostream>
#include <vector>
#include <list>
#include "vector.h"
#include "allocator.h"
#include "lf_uninitialized.h"
#include "lf_algorithm.h"
#include "lf_list.h"
#include "lf_deque.h"

using namespace std;

void print(const lf::vector<int> vec) {
    for(size_t i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}

int main() {

    lf::deque<int> dq;
    for(int i = 0; i < 1000; i++)
        dq.push_front(i);

    for(auto it = dq.begin(); it != dq.end(); ++it)
        cout << *it << endl;
    cout << dq.size() << endl;

    for(int i = 0; i < 990; i++)
        dq.pop_back();

    for(auto it = dq.begin(); it != dq.end(); ++it)
        cout << *it << endl;
    cout << dq.size() << endl << endl;

    dq.erase(dq.begin() + 1, dq.begin() + 8);
    for(auto it = dq.begin(); it != dq.end(); ++it)
        cout << *it << endl;
    cout << dq.size() << endl << endl;

    dq.insert(dq.begin() + 1, 20);
    for(auto it = dq.begin(); it != dq.end(); ++it)
        cout << *it << endl;
    cout << dq.size() << endl << endl;

    return 0;
}