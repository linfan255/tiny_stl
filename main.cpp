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

using namespace std;

void print(const lf::vector<int> vec) {
    for(size_t i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}

int main() {

    lf::stack<double> st;
    cout << st.size() << endl;

    st.push(1);
    st.push(3);
    st.push(5);
    st.push(7);

    cout << st.size() << endl;
    cout << st.top() << endl;

    st.pop();   cout << st.top() << endl;
    st.pop();   cout << st.top() << endl;
    st.pop();   cout << st.top() << endl;
    cout << st.size() << endl;
    return 0;
}