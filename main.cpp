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
#include "lf_map.h"

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

    lf::map<int,double> m;
    int var1 = 1;
    double var2 = 1.50;
    for(int i = 0; i < 100000; i++)
        m.insert(lf::pair<int,double>(i + var1, i + var2));

    m.erase(m.begin());
    for(auto it = m.begin(); it != m.end(); ++it)
        cout << it->first << "--" << it->second << endl;

    return 0;
}