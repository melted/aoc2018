#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

void solve(vector<pair<int, int>>& points) {
    vector<int> scores(points.size());
    int region = 0;
    auto p_x = minmax_element(points.begin(), points.end(),
        [](auto& p1, auto& p2) { return p1.first < p2.first; });
    auto p_y = minmax_element(points.begin(), points.end(),
        [](auto& p1, auto& p2) { return p1.second < p2.second; });

    int min_x = (*p_x.first).first;
    int max_x = (*p_x.second).first;
    int min_y = (*p_y.first).second;
    int max_y = (*p_y.second).second;

    for (int x = min_x; x < max_x+1; x++) {
        for (int y = min_y; y < max_y+1; y++) {
            vector<int> dists(points.size());
            transform(points.begin(), points.end(), dists.begin(),
                [x,y](auto& p) {
                    return abs(p.first - x) + abs(p.second -y);
                });
            auto p = min_element(dists.begin(), dists.end());
            auto sum = accumulate(dists.begin(), dists.end(), 0);
            if (sum < 10000) region++;
            if (count(dists.begin(), dists.end(), *p) == 1) {
                auto i = p - dists.begin();
                if (x == min_x || x == max_x || y == min_y || y == max_y) {
                    scores[i] = -1;
                } else {
                    if (scores[i] != -1) scores[i]++;
                }
            }
        }
    }
    cout << *max_element(scores.begin(), scores.end()) << endl;
    cout << region << endl;
}

int main(int argc, char** argv) {
    ifstream input("data/input6.txt");
    vector<pair<int, int>> points;
    do {
        int x = -1 , y = -1;
        string s; // comma
        input >> x >> s >> y;
        if (x > 0) points.emplace_back(x, y);
    } while (!input.eof());
    solve(points);
}