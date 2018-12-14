#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct state {
    int elf1;
    int elf2;
    vector<char> recipes;

    state() : elf1(0), elf2(1), recipes({ 3, 7 }) { }

    void one_round() {
        int nxt = recipes[elf1] + recipes[elf2];
        if (nxt > 9) {
            recipes.push_back(1);
            recipes.push_back(nxt-10);
        } else {
            recipes.push_back(nxt);
        }
        elf1 = (elf1 + recipes[elf1] + 1) % recipes.size();
        elf2 = (elf2 + recipes[elf2] + 1) % recipes.size();
    }
};

void solve(int t, int n) {
    state s;
    while (s.recipes.size() < t+n) {
        s.one_round();
    }
    for(int i = 0; i < n; i++) cout << int(s.recipes[t+i]);
    cout << endl;
}

void solve2() {
    state s;
    vector<char> target({ 5, 4, 0, 3, 9, 1});
    int offset;
    while (true) {
        s.one_round();
        for (int i : { 0,1 })
            if (mismatch(s.recipes.end()-target.size()-i,
                         s.recipes.end()-i,
                        target.begin()).first == s.recipes.end()-i ) {
                cout << s.recipes.size() - target.size() -i << endl;
                return;
            }
    }
}

int main() {
    solve(540391, 10);
    solve2();
}