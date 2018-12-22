#include <vector>
#include <iostream>
#include <stdint.h>
#include <map>
#include <tuple>
#include <set>

using namespace std;

using chart = vector<vector<int>>;

int erosion(int n) {
    return (n + 11820)%20183;
}

vector<tuple<int, int, int>> get_neighbors(chart& area, int x, int y) {
    vector<tuple<int, int, int>> out;
    if (x > 0) out.push_back({ x-1, y, erosion(area[y][x-1])%3 });
    if (y > 0) out.push_back({ x, y-1, erosion(area[y-1][x])%3 });
    if (x < area[0].size()-1) out.push_back({ x+1, y, erosion(area[y][x+1])%3 });
    if (y < area.size() -1) out.push_back({ x, y+1, erosion(area[y+1][x])%3 });
    return out;
}

int find(chart& area, int tx, int ty) {
    int t = 0;
    set<tuple<int, int, int>> traversed;
    map<int, vector<tuple<int, int, int>>> events = { { 0, { { 0, 0, 1 }}}};
    while (true) {
        for (auto [x, y, e] : events[t]) {
            if (x == tx && y == ty) {
                if (e == 2) events[t+7].push_back({ x, y, 1 }); else return t;
            }
        //    cout << t << ": "<< x << " " << y << " " << e << endl; 
            if (traversed.count({ x, y, e})) continue;
            for (auto [nx, ny, env] : get_neighbors(area, x, y)) {
                int dt = 1 + (e == env?7:0);
                int ce = erosion(area[y][x])%3;
                int de = e;
                if (e == env) {
                    set<int> tools = {0,1,2};
                    tools.erase(env);
                    tools.erase(ce);
                    de = *tools.begin();
                }
                events[t+dt].push_back({ nx, ny, de });
            }
            traversed.insert({ x, y, e});
        };
        t++;
        if (events.size() == 0) {
            cout << "nowhere left to go!" << endl;
            break;
        }
    }
    return t;
}

int main() {
    uint64_t depth = 11820;
    int tx = 7;
    int ty = 782;
    int risk = 0;
    chart area(ty*2);
    for (auto& v : area) v.resize(tx*10);
    area[0][0] = 0;
    for (int i = 0; i < tx*10; i++) {
        for (int j = 0; j < ty*2; j++) {
            if (j == 0) area[0][i] = (i * 16807)%20183;
            else if (i == 0)  area[j][0] = (j * 48271)%20183;
            else area[j][i] = erosion(area[j-1][i]) * erosion(area[j][i-1]);
            if (i == tx && j == ty) area[j][i] = 0;
        }
    }
    for (int i = 0; i <= tx; i++)
        for (int j = 0; j <= ty; j++)
            risk += erosion(area[j][i])%3;

    cout << risk << endl;
    int t = find(area, tx, ty);
    cout << t << endl;
}