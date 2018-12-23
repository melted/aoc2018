#include <tuple>
#include <vector>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

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

void solve1() {
    auto m = max_element(bots.begin(), bots.end(), [](bot& a, bot& b) {
        return get<3>(a) < get<3>(b);
    });
    auto [ox, oy, oz, r] = *m;
    int count = 0;
    for (auto& [x,y,z,rr] : bots) {
        int64_t d = abs(x-ox) + abs(y-oy) + abs(z-oz);
        if (d <= r) count++;
    }
    cout << count << endl;
}

int main() {
    read_data();
    solve1();
    cout << bots.size() << endl;
}