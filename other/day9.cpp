#include <list>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <algorithm>

using namespace std;

void move(list<int>& l, list<int>::iterator& iter, int steps) {
    int dir = steps < 0 ? -1 : 1;
    for (int i = 0; i != steps; i += dir) {
        if (iter == begin(l) && dir < 0) iter = end(l);
        if (dir > 0) iter++; else iter--;
        if (iter == end(l) && dir > 0) iter = begin(l);
    }
}

uint64_t solve(int players, int max_marble) {
    vector<uint64_t> scores(players);
    list<int> circle = { 0 };
    list<int>::iterator current_marble = begin(circle);
    int current_player = 0;
    for (int i = 1; i <= max_marble; i++) {
        if (i % 23 != 0) {
            move(circle, current_marble, 2);
            circle.insert(current_marble, i);
            move(circle, current_marble, -1);
        } else {
            scores[current_player] += i;
            move(circle, current_marble, -7);
            scores[current_player] += *current_marble;
            current_marble = circle.erase(current_marble);
            if (current_marble == end(circle)) current_marble = begin(circle);
        }
        ++current_player %= players;
    }
    return *max_element(begin(scores), end(scores));
}

int main() {
    uint64_t result = solve(430, 71588);
    uint64_t result2 = solve(430, 7158800);
    cout << result << " " << result2 << endl;
}