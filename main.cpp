#include <iostream>
#include <vector>
#include <list>
#include "vector.h"
#include "allocator.h"
#include "lf_uninitialized.h"
#include "lf_algorithm.h"
#include "lf_list.h"
#include "lf_deque.h"
#include "lf_stack.h"
#include "lf_avl_tree.h"

using namespace std;

void print(const lf::vector<int> vec) {
    for(size_t i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}

template <typename T>
struct identity {
    const T& operator()(const T& x) const {
        return x;
    }
};

template <typename T>
struct mless {
    bool operator()(const T& x, const T& y) const {
        //cout << "calls mless" << endl;
        return x < y;
    }
};

int main() {


    lf::avl_tree<int, int, identity<int>, mless<int> > at;
    for(int i = 1; i <= 5; i++)
        at.insert_unique(i);

    cout << "size=" << at.size() << endl;

    for(auto it = at.begin(); it != at.end(); it++)
        cout << *it << " ";
    cout << endl;

    auto it = at.end();
    it--;
    at.erase(it);

    for(auto it = at.begin(); it != at.end(); it++)
        cout << *it << " ";
    cout << endl;

    it = at.end();
    at.erase(--it);
    for(auto it = at.begin(); it != at.end(); it++)
        cout << *it << " ";
    cout << endl;

    it = at.end();
    at.erase(--it);
    for(auto it = at.begin(); it != at.end(); it++)
        cout << *it << " ";
    cout << endl;

    at.erase(at.begin());
    for(auto it = at.begin(); it != at.end(); it++)
        cout << *it << " ";
    cout << endl;

    cout << at.size() << endl;
    return 0;
}