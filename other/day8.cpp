#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <deque>
#include <iterator>

using namespace std;

struct tree {
    vector<tree> children;
    vector<int> metadata;
};

tree treeify(deque<int>& data) {
    int n = data.front();
    data.pop_front();
    int m = data.front();
    data.pop_front();
    vector<tree> children;
    for (int i = 0; i < n; i++) {
        children.push_back(treeify(data));
    }
    vector<int> metadata;
    copy(begin(data), begin(data)+m, back_inserter(metadata));
    data.erase(begin(data), begin(data)+m);
    return { children, metadata };
}

int sum_metadata(const tree& t) {
    int sum = accumulate(begin(t.metadata), end(t.metadata), 0);
    for (const auto& tt : t.children) {
        sum += sum_metadata(tt);
    }
    return sum;
}

int silly_sum(const tree& t) {
    if (t.children.size() == 0) {
        return accumulate(begin(t.metadata), end(t.metadata), 0);
    } else {
        int sum = 0;
        for (int i : t.metadata) {
            i--;
            if (i >= 0 && i < t.children.size()) {
                sum += silly_sum(t.children[i]);
            }
        }
        return sum;
    }
}

int main() {
    ifstream input("data/input8.txt");
    deque<int> data;
    while (!input.eof()) {
        int d = -1;
        input >> d;
        if (d != -1) data.push_back(d);
    }
    tree t = treeify(data);
    int result = sum_metadata(t);
    cout << result << endl;
    int result2 = silly_sum(t);
    cout << result2 << endl;
    return 0;
}