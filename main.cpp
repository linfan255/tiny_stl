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
#include "lf_set.h"

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

    lf::multiset<int> s;
    for(int i = 100; i >= 0; --i)
        s.insert(5);

    for(auto it = s.begin(); it != s.end(); ++it)
        cout << *it << endl;

    return 0;
}