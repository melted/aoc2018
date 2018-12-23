#include <tuple>
#include <vector>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstdint>

using namespace std;
using bot = tuple<int64_t, int64_t, int64_t, int64_t>;

vector<bot> bots;

void read_data() {
    FILE* input = fopen("data/input23.txt", "r");
    while(!feof(input)) {
        int x,y,z,r;
        int res = fscanf(input, "pos=<%d,%d,%d>, r=%d ", &x, &y, &z, &r);
        if (res == 4) bots.push_back({ x, y, z, r}); 
    }
    fclose(input);
}

int64_t dist(bot& a, bot& b) {
    auto& [ax, ay, az, ar] = a;
    auto& [bx, by, bz, br] = b;
    return abs(ax-bx) + abs(ay-by) + abs(az-bz);
}

bool in_range(bot& a, bot& b) {
    return get<3>(a) >= dist(a,b);
}

int bots_in_range(bot& b) {
    return count_if(bots.begin(), bots.end(), [&b](bot& a) { return in_range(a, b); });
}

void solve1() {
    auto m = max_element(bots.begin(), bots.end(), [](bot& a, bot& b) {
        return get<3>(a) < get<3>(b);
    });
    auto [ox, oy, oz, r] = *m;
    int count = 0;
    for (auto& [x,y,z,rr] : bots) {
        int d = abs(x-ox) + abs(y-oy) + abs(z-oz);
        if (d <= r) count++;
    }
    cout << count << endl;
}

void dump(bot& b) {
    auto& [bx, by, bz, br] = b;
    cout << bx << "," << by << "," << bz << "," << br << endl;
}

bot find_point() {
    int step = 100000000;
    int round = 1;
    bot o = { 0,0,0,0 };
    bot tp = { 0,0,0,0 };
    while (true) {
        bot best(tp);
        int bc = 0;
        for (int i = -20; i <= 20; i++) {
            for (int j = -20; j <= 20; j++) {
                for (int k = -20; k <= 20; k++) {
                    bot next(tp);
                    auto& [x, y, z, rr] = next;
                    x += i*step;
                    y += j*step;
                    z += k*step;
                    rr = bots_in_range(next);
                    if (rr > bc || (rr == bc && dist(next, o) < dist(best,o))) {
                        cout << "best so far ";
                        dump(next);
                        best = next;
                        bc = rr;
                    } 
                }
            }    
        }
        cout << step << endl;
        dump(best);
        tp = best;
        if (step == 1) break;
        step /= 10;
    }
    return tp;
}

int main() {
    read_data();
    solve1();
    bot o = { 0,0,0,0 };
    bot b = find_point();
    cout << dist(b, o) << endl;
    dump(b);
    cout << bots.size() << endl;
}