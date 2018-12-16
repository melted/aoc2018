#include <vector>
#include <tuple>
#include <iostream>
#include <cstdio>
#include <functional>
#include <set>

using namespace std;

using instr = tuple<int, int, int, int>;
using opf = function<void(int, int, int, vector<int>&)>;

vector<instr> prog;

struct testcase {
    vector<int> before;
    vector<int> after;
    instr test;
};

vector<testcase> tests;

void addr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] + regs[b];
}

void addi(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] + b;
}

void mulr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] * regs[b];
}

void muli(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] * b;
}

void banr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] & regs[b];
}

void bani(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] & b;
}

void borr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] | regs[b];
}

void bori(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] | b;
}

void setr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a];
}

void seti(int a, int b, int c, vector<int>& regs) {
    regs[c] = a;
}

void gtir(int a, int b, int c, vector<int>& regs) {
    regs[c] = a > regs[b] ? 1:0;
}

void gtri(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] > b ? 1:0;
}
void gtrr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] > regs[b] ? 1:0;
}


void eqir(int a, int b, int c, vector<int>& regs) {
    regs[c] = a == regs[b] ? 1:0;
}

void eqri(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] == b ? 1:0;
}
void eqrr(int a, int b, int c, vector<int>& regs) {
    regs[c] = regs[a] == regs[b] ? 1:0;
}

vector<opf> ops = { addr, addi, mulr, muli, banr, bani, borr, bori,
                    setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr };

int solve1() {
    int count = 0;
    for (auto& t : tests) {
        int n = 0;
        for (auto f : ops) {
            vector<int> regs(t.before);
            auto [i, a, b, c] = t.test;
            f(a, b, c, regs);
            if (regs == t.after) n++;
        }
        if (n > 2) count++;
    }
    return count;
}

vector<opf> opcodes(16);

void map_opcodes() {
    vector<set<int>> mapping(16);
    for (auto& s : mapping) {
        for(int i = 0; i < 16; i++) s.insert(i);
    }
    for (auto& t : tests) {
        for (int n = 0; n < 16; n++) {
            vector<int> regs(t.before);
            auto [i, a, b, c] = t.test;
            ops[n](a, b, c, regs);
            if (regs != t.after) mapping[i].erase(n);
        }
    }
    set<int> settled;
    while(settled.size() < 16) {
        for (int i = 0; i < 16; i++) {
            if (settled.count(i)) continue;
            if (mapping[i].size() == 0) {
                cout << "No mapping for " << i << endl;
            }
            if (mapping[i].size() == 1) {
                int n = *(mapping[i].begin());
                settled.insert(i);
                opcodes[i] = ops[n];
                for (auto& s : mapping) {
                    s.erase(n);
                }
            }
        }
    }
}

int execute(vector<instr>& p) {
    vector<int> regs(4);
    for (auto& [i, a, b, c] : p) {
        opcodes[i](a, b, c, regs);
    }
    return regs[0];
}

void read_data() {
    FILE* input = fopen("data/input16.txt", "r");
    while (true) {
        vector<int> before(4);
        vector<int> after(4);
        int i, a, b, c;
        int res = fscanf(input, "Before: [%d, %d, %d, %d] ", &before.data()[0], &before.data()[1], &before.data()[2], &before.data()[3]);
        if (res < 4) break;
        res = fscanf(input, "%d %d %d %d ", &i, &a, &b, &c);
        if (res < 4) {
            cout << "unexpected input!" << endl;
            continue;
        }
        res = fscanf(input, "After: [%d, %d, %d, %d] ", &after.data()[0], &after.data()[1], &after.data()[2], &after.data()[3]);
        if (res < 4) {
            cout << "unexpected input!" << endl;
            continue;
        }
        tests.push_back({ before, after, { i, a, b, c}});
    }
    while(!feof(input)) {
        int i, a, b, c;
        int res = fscanf(input, "%d %d %d %d ", &i, &a, &b, &c);
        if (res == 4) prog.push_back({ i, a, b, c });
    }
    fclose(input);
}

int main() {
    read_data();
    int result = solve1();
    cout << result << endl;
    map_opcodes();
    int result2 = execute(prog);
    cout << result2 << endl;
    return 0;
}