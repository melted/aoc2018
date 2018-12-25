#include <vector>
#include <iostream>
#include <tuple>
#include <cstdio>

using namespace std;

using point = tuple<int, int, int, int>;

vector<vector<point>> cs;

vector<point> points;

void read_data() {
    FILE* input = fopen("data/input25.txt", "r");
    while(!feof(input)) {
        int x, y, z, w;
        int res = fscanf(input, "%d,%d,%d,%d ", &x, &y, &z, &w);
        if (res == 4) cs.push_back({{ x,y,z,w }});
    }
}

bool in_range(point& a, point& b) {
    auto& [ax, ay, az, aw] = a;
    auto& [bx, by, bz, bw] = b;
    return abs(ax-bx) + abs(ay-by) + abs(az-bz) + abs(aw-bw) < 4;
}

void merge() {
    for (auto it = cs.begin(); it != cs.end(); it++) {
        for (auto jt = it+1; jt != cs.end(); jt++) {
            for (auto& p : *it) {
                for (auto& q : *jt) {
                    if (in_range(p,q)) {
                        it->insert(it->end(), jt->begin(), jt->end());
                        jt = cs.erase(jt);
                        goto done;
                    }
                }
            }
done:
        while (false); // no-op
        }
    }
}

int main() {
    read_data();
    int count = cs.size();
    while (true) {
        merge();
        int next = cs.size();
        if (next == count) break;
        count = next;
    }
    cout << count << endl;
}