#include <iostream>
#include <vector>
#include "vector.h"
#include "allocator.h"
#include "lf_uninitialized.h"
#include "lf_algorithm.h"

using namespace std;

void print(const lf::vector<int> vec) {
    for(size_t i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}

int main() {

    lf::vector<int> vec;
    vec.resize(10, 23);
    cout << vec.size() << endl;
    cout << vec.capacity() << endl;

    vec.insert(vec.begin() + 5, 5, 520);
    print(vec);

    vec.erase(vec.begin());
    print(vec);

    lf::vector<int> copyVec;
    copyVec.resize(20);
    copy(vec.begin(), vec.end(), copyVec.begin());
    print(copyVec);

    for(int i = 0; i < 10000000; i++)
        copyVec.push_back(i);
    cout << "after push 10000000 elements, size() = " << copyVec.size()
                                                      << " capacity() = " << copyVec.capacity()
                                                                          << endl;

    return 0;
}