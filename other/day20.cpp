#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <tuple>
#include <set>

using namespace std;

using pos = pair<int, int>;
using chart = vector<vector<set<pos>>>;

chart area;

map<int, pos> offset = {
    {'N', {0, -1}},
    { 'E', {1, 0}},
    { 'W', {-1, 0}},
    { 'S', {0, 1}}
};

string read_data() {
    ifstream input("data/input20.txt");
    string out;
    getline(input, out);
    return out;
}

set<pos>& get(pos p) {
    return area[p.second][p.first];
}

set<pos> parse(istringstream& sin, pos start) {
    set<pos> terms = { start };
    while(true) {
        int ch = sin.get();
        if (ch == '^') {
        } else if (ch == '(') {
            set<pos> next;
            for (pos p : terms) {
                auto t = parse(sin, p);
                next.insert(t.begin(), t.end());
            }
            terms = next;
        } else if (ch == -1 || ch == ')' || ch == '$' || sin.eof()) {
            break;
        } else if (ch == '|') {
            auto adds = parse(sin, start);
            terms.insert(adds.begin(), adds.end());
            break;
        } else {
            if (terms.size() != 1) cout << "confused" << endl;
            pos os = offset[ch];
            pos prev = *(terms.begin());
            pos np = { prev.first+os.first, prev.second+os.second };
            set<pos>& n = get(np);
            n.insert(prev);
            set<pos>& p = get(prev);
            p.insert(np);
            terms = { np };
        }
    }
    return terms;
}

pair<int, int> cost(pos p) {
    set<pos> handled;
    set<pos> to_handle = { p };
    int cost = 0;
    int expensive = 0;
    while(to_handle.size()) {
        set<pos> next;
        for(const pos& pt : to_handle) {
            for (auto z : get(pt)) {
                if (!handled.count(z)) next.insert(z);
            }
            if (cost >= 1000) expensive++;
        }
        handled.insert(to_handle.begin(), to_handle.end());
        to_handle = next;
        cost++;
    }
    return { cost-1, expensive };
}

pair<int, int> solve(string s) {
    area.clear();
    area.resize(200);
    for (auto& r : area) r.resize(200);
    istringstream stream(s);
    auto terminals = parse(stream, { 100, 100 });
    return cost({ 100, 100});
}

int main() {
    string data = read_data();
    auto [res, res2] = solve(data);
    cout << res << " " << res2 << endl;
}