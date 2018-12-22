#include <vector>
#include <iostream>
#include <stdint.h>

using namespace std;

using chart = vector<vector<uint64_t>>;
int main() {
    uint64_t depth = 11820;
    int tx = 7;
    int ty = 782;
    int risk = 0;
    chart area(ty+1);
    for (auto& v : area) v.resize(tx+1);
    auto erosion = [depth](auto n){ return (n+depth)%20183; };
    area[0][0] = 0;
    for (int j = 1; j <= tx; j++) area[0][j] = (j * 16807)%20183;
    for (int j = 1; j <= ty; j++) area[j][0] = (j * 48271)%20183;
    for (int i = 1; i <= tx; i++) {
        for (int j = 1; j <= ty; j++) {
            area[j][i] = erosion(area[j-1][i]) * erosion(area[j][i-1]);
  //          cout << i << " "<< j << " " << area[j][i] << endl;
        }
    }
    area[ty][tx] = 0;

    for (auto& v : area) {
        for (auto n : v) {
            risk += erosion(n) % 3;
        }
    }
    cout << risk << endl;
}