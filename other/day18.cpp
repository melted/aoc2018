#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>

using namespace std;

vector<string> area;

unordered_map<string, int> states;
unordered_map<int, int> scores;

void read_map() {
    ifstream input("data/input18.txt");
    while (!input.eof()) {
        string s;
        getline(input, s);
        if (s.length()) area.push_back(s);
    }
}

int neighbors(int x, int y, char c) {
    vector<pair<int, int>> cells = {
        { x-1, y-1 }, { x, y-1 }, { x+1, y-1 }, { x-1, y }, { x+1, y },
        { x-1, y+1}, { x, y+1 }, {x+1,y+1}
    };
    int count = 0;
    for (auto [nx, ny] : cells) {
        if (ny >= 0 && ny < area.size() && nx >= 0 && nx < area[ny].size()) {
            if (area[ny][nx] == c) count++;
        }
    }
    return count;
}

int snapshot(int i) {
    string state;
    for(string& s : area) state.append(s);
    if (states.count(state) == 1) return states[state];
    states.insert({ state, i });
    return -1;
}

void evolve() {
    vector<string> next(area);
    for (int i = 0; i < area.size(); i++) {
        for (int j = 0; j < area[i].size(); j++) {
            if (area[i][j] == '.') {
                if (neighbors(j, i, '|') > 2) next[i][j] = '|';
            } else if (area[i][j] == '|') {
                if (neighbors(j, i, '#') > 2) next[i][j] = '#';
            } else if (area[i][j] == '#') {
                if (!neighbors(j, i, '|') || !neighbors(j,i,'#')) next[i][j] = '.';
            }
        }
    }
    area = next;
}

void dump() {
    for (string& s : area) {
        cout << s << endl;
    }
    cout << "\n\n";
}

int census() {
    int trees = 0;
    int lumber = 0;
    for (string& s : area) {
        trees += count(s.begin(), s.end(), '|');
        lumber += count(s.begin(), s.end(), '#');
    }
    return trees*lumber;
}

int main() {
    read_map();
    for (int i = 0; i < 1000000000; i++) {
        int prev = snapshot(i);
        if (prev < 0) {
            evolve();
            scores[i] = census();
        } else {
            int cycle = i - prev;
            int target = (1000000000 - i -1)%cycle + prev;
            cout << scores[9] << endl;
            cout << scores[target]<< endl;
            break;
        }
    }
}