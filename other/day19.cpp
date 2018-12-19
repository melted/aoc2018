#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <string>

using namespace std;
using opf = function<void(int, int, int, vector<int>&)>;
using instr = tuple<opf, int, int, int>;

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

map<string, opf> opcodes = {
    { "addr", addr },
    { "addi", addi },
    { "mulr", mulr },
    { "muli", muli },
    { "banr", banr },
    { "bani", bani },
    { "borr", borr },
    { "bori", bori },
    { "setr", setr },
    { "seti", seti },
    { "gtir", gtir },
    { "gtri", gtri },
    { "gtrr", gtrr },
    { "eqir", eqir },
    { "eqri", eqri },
    { "eqrr", eqrr },
};

vector<instr> prog;
int pc;

void read_prog() {
    ifstream input("data/input19.txt");
    while (!input.eof()) {
        string s;
        input >> s;
        if (s == "#ip") {
            input >> pc;
        } else if (!s.empty()) {
            int a, b, c;
            input >> a >> b >> c;
            prog.push_back({ opcodes[s], a, b, c });
        }
    }
}

void dump(vector<int> r) {
    for (int i : r) cout << i << " ";
    cout << endl;
}

int execute(vector<instr> prog, int pctr) {
    vector<int> regs(6);
    int counter = 0;
    regs[0] = 0;
    while (regs[pctr] >= 0 && regs[pctr] < prog.size()) {
        auto [fn, a, b, c] = prog[regs[pctr]];
        fn(a, b, c, regs);
        regs[pctr]++;
        counter++;
        cout << counter << ": ";
        dump(regs);
    }
    cout << counter << endl;
    return regs[0];
}


int main() {
    read_prog();
    cout << prog.size() << endl;
    int res = execute(prog, pc);
    cout << res << endl;
}