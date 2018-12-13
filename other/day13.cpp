#include <vector>
#include <tuple>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

const int north = 1;
const int east = 1 << 1;
const int south = 1 << 2;
const int west = 1 << 3;

vector<vector<int>> tracks;

struct cart {
    int x;
    int y;
    int dir;
    int cycle;
    int id;
    bool dead;

    void turn() {
        switch (cycle) {
            case 0:
                dir >>= 1;
                if (dir == 0) dir = west;
                break;
            case 1:
                break;
            case 2:
                dir <<= 1;
                if (dir == 16) dir = north;
                break;
        }
        ++cycle %= 3;
    }

    void move() {
        int t = tracks[y][x];
        if (t == 15) {
            turn();
        } else if (!(t & dir)) {
            if (dir & (east | west)) {
                dir = (north | south) & t;
            } else if (dir & (north | south)) {
                dir = (east | west) & t;
            }
        }
        if (dir == north) y -= 1;
        if (dir == south) y += 1;
        if (dir == west) x -= 1;
        if (dir == east) x += 1;
    }
};

vector<cart> carts;

bool compare_carts(cart& a, cart& b) {
    if (a.y < b.y) return true;
    if (a.y == b.y && a.x < b.x) return true;
    return false;
}

void read_data() {
    int x = 0, y = 0;
    vector<int> current;
    ifstream input("data/input13.txt");
    int last = 0;
    int id = 0;
    while(!input.eof()) {
        int c = input.get();
        switch(c) {
            case '-':
                current.push_back(west | east);
                break;
            case '|':
                current.push_back(north | south);
                break;
            case '\\':
                if (last == '-' || last == '+' )
                    current.push_back(west | south);
                else
                    current.push_back(east | north);
                break;
            case '/':
                if (last == '-' || last == '+' )
                    current.push_back(west | north);
                else
                    current.push_back(east | south);
                break;
            case '+':
                current.push_back(west | east | north | south);
                break;
            case '\n':
                tracks.push_back(current);
                current.clear();
                y++;
                break;
            case '>':
                current.push_back(west | east);
                carts.push_back({ x, y, east, 0, id++, false });
                break;
            case '<':
                current.push_back(west | east);
                carts.push_back({ x, y, west, 0, id++, false });
                break;
            case '^':
                current.push_back(north | south);
                carts.push_back({ x, y, north, 0, id++, false});
                break;
            case 'v':
                current.push_back(north | south);
                carts.push_back({ x, y, south, 0, id++, false });
                break;
            default:
                current.push_back(0);
                break;
        }
        last = c;
        x = current.size();
    }
}

bool run_tick() {
    sort(carts.begin(), carts.end(), compare_carts);
    static bool first = true;
    for (cart& c : carts) {
        if (c.dead) continue;
        c.move();
        auto it = carts.begin();
        int n = count_if(carts.begin(), carts.end(), [&c](cart& a){ return (c.x == a.x) && (c.y == a.y); });

        if (n > 1) {
            for (auto it = carts.begin(); it != carts.end(); it++) {
                if (it->x == c.x && it->y == c.y) {
                    it->dead = true;
                }
            }
            if (first) {
                first = false;
                cout << c.x << ", " << c.y << endl;
            }
        }
    }
    auto it = carts.begin();
    while (it != carts.end()) {
        if (it->dead) it = carts.erase(it); else it++;
    }
    if (carts.size() == 1) {
        cout << carts[0].x << "," << carts[0].y << endl;
        return true;
    }
    return false;
}

int main() {
    read_data();
    while(!run_tick());
}