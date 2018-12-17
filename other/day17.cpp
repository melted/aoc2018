#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

using chart = vector<vector<char>>;

chart area(2000);
int min_y, max_y;

void read_data() {
    FILE* input = fopen("data/input17.txt", "r");
    for(auto& v: area) v.resize(1000);
    int miny = INT_MAX;
    int maxy = 0;
    while(!feof(input)) {
        int a, b, c;
        int res = fscanf(input, "x=%d, y=%d..%d ", &a, &b, &c);
        if (res == 3) {
            for(int i=b; i < c+1; i++) area[i][a] = 3;
            if (b<miny) miny = b;
            if (c>maxy) maxy = c;
        } else {
            res = fscanf(input, "y=%d, x=%d..%d ", &a, &b, &c);
            if (res == 3) {
                for(int i=b; i < c+1; i++) area[a][i] = 3;
                if (a<miny) miny = a;
                if (a>maxy) maxy = a;
            }
        }
    }
    min_y = miny;
    max_y = maxy;
    fclose(input);
}

void drop(int x, int y);
void spill(int x, int y) {
    int l = x-1;
    int r = x+1;
    bool lw = false, rw = false;
    bool lh = false, rh = false;
    area[y][x] = 1;
    while (!(rh || rw) || !(lw || lh)) {
        if (l<0) return;
        if (r>999) return;
        lw = area[y][l] == 3;
        rw = area[y][r] == 3;
        lh = area[y+1][l] < 2;
        rh = area[y+1][r] < 2;

        if (!lw) area[y][l] = 1;
        if (!rw) area[y][r] = 1;

        if (!(lh || lw)) l--;
        if (!(rh || rw)) r++;
    }
    if (lw && rw) {
        for (int i = l+1; i < r; i++) area[y][i]=2;
        spill(x, y-1);
    }
    if (lh) drop(l, y);
    if (rh) drop(r, y);
}

void drop(int x, int y) {
    while (area[y+1][x] == 0) {
        y++;
        area[y][x] = 1;
        if (y >= max_y) return;
    }
//    cout << y << " " << x << endl;
    if (area[y+1][x]>1) spill(x,y);
}


void fill() {
    drop(500, 0);
}

void dump() {
    for (auto& r : area) {
        for (char c : r) {
            cout << (c == 0 ?".":c==1?"|":c==2?"&":"=");
        }
        cout << "\n";
    }
}

pair<int, int> census() {
    int count = 0;
    int retained = 0;
    for (int i = min_y; i <= max_y; i++ ) {
        for(char c : area[i]) {
            if (c == 1 || c == 2) count++;
            if (c == 2) retained++;
        }
    }
    return  { count, retained };
}

int main() {
    read_data();
    fill();
    auto [res, res2] = census();
    cout << res << " "<< res2 << endl;
}