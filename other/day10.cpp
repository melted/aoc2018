#include <vector>
#include <set>
#include <iostream>
#include <tuple>
#include <cstdio>
#include <algorithm>
#include <string>

using namespace std;

using v2 = pair<int, int>;

struct point {
    int x;
    int y;
    int dx;
    int dy;

    v2 at(int t) const {
        return { x+t*dx, y+t*dy };
    }
};

set<v2> current_state(const vector<point>& points, int t) {
    set<v2> state;
    for(auto& p : points) {
        state.insert(p.at(t));
    }
    return state;
}

bool check(vector<point>& points, int t) {
    auto mm = minmax_element(begin(points), end(points),
         [t](const point& a, const point& b) { return a.at(t).first < b.at(t).first; });
    if ((*mm.second).x - (*mm.first).x > 300) return false;
    set<v2> state = current_state(points, t);
    int n = 0;
    for (const v2& p : state) {
        if (state.count({ p.first-1, p.second})) n++;
        if (state.count({ p.first+1, p.second})) n++;
        if (state.count({ p.first, p.second-1})) n++;
        if (state.count({ p.first, p.second+1})) n++;
    }
    return n > points.size()/2;
}

int main() {
    vector<point> data;
    FILE* input = fopen("data/input10.txt", "r");
    while (!feof(input)) {
        int x, y, dx, dy;
        int res = fscanf(input, "position=<%d, %d> velocity=<%d, %d> ", &x, &y, &dx, &dy);
        if (res == 4) data.push_back({ x, y, dx, dy });
    }
    fclose(input);
    for (int i = 0; i < 20000; i++) {
        if (check(data, i)) {
            set<v2> state = current_state(data, i);
            auto mx = minmax_element(begin(state), end(state),
                 [](const v2& a, const v2& b) { return a.first < b.first; });
            auto my = minmax_element(begin(state), end(state),
                 [](const v2& a, const v2& b) { return a.second < b.second; });
            int min_x = (*mx.first).first;
            int d_x = (*mx.second).first - min_x;
            int min_y = (*my.first).second;
            int d_y = (*my.second).second - min_y;
            vector<string> image(d_y+1);
            for (string& s : image) {
                s.resize(d_x+1, ' ');
            }
            for (auto& p : state) {
                image[p.second - min_y].at(p.first-min_x) = '#';
            }
            for_each(image.begin(), image.end(), [](string& s) { cout << s << endl; });
            cout << i << endl;
            break;
        }
    }
}