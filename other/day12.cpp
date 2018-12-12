#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <bitset>
using namespace std;

const uint64_t long_run = 50000000000;

using arena=bitset<1024>;
unordered_map<arena, uint64_t> states;


arena evolve(arena& state) {
    static vector<int> rules = { 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0,
                                 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
    arena next;
    if (state[2] || state[3] || state[1020] || state[1021]) cout << "overflow!" << endl;
    for (int i = 2; i < 1022; i++) {
        int vicinity = 0;
        for (int n : {0,1,2,3,4}) vicinity += state[i+n-2] ? 1<<n:0;
        if (rules[vicinity]) next[i] = true;
    }
    return next;
}

uint64_t sum(arena& state) {
    uint64_t sum = 0;
    for (int i = 0; i < 1024; i++) if (state[i]) sum += i - 512;
    return sum;
}

int main() {
    set<int> init_state = { 1, 2, 5, 7, 10, 11, 14, 15, 18, 19, 23, 24, 25, 26, 27,
                        29, 35, 38, 41, 42, 44, 45, 46, 48, 50, 51, 52, 53,
                        60, 68, 71, 72, 73, 75, 77, 79, 80, 82, 84, 86, 87, 88,
                        92, 93, 95, 96, 97, 99 };

    arena state;
    for (int n : init_state) {
        state[n+512] = true;
    }
    for (uint64_t i = 0; i < long_run; i++) {
        if (states.count(state)) {
            auto first = states[state];
            uint64_t cycle = i - first;
            uint64_t shifts = (long_run - i) / cycle;
            uint64_t target = (long_run - i) % cycle;
            cout << sum(state)+state.count()*shifts << endl;
            break;
        }
        states.insert({state<<1, i});
        int score = sum(state);
        if (i == 20) cout << score << endl;
        state = evolve(state);
    }
    return 0;
}