#include <iostream>
#include <set>

using namespace std;

int main() {
    int r3 = 0 , r4 = 0, r5 = 0;
    set<int> seen;
    int last = 0;
    bool first = true;
    while (true) {
        last = r4;
        r3 = r4 | 65536;
        r4 = 3730679;
        while (true) {
            r5 = r3 & 255;
            r4 = r4 + r5;
            r4 &= 16777215;
            r4 *= 65899;
            r4 &= 16777215;
            if (r3 < 256) break;
            r3 /= 256; 
        }
        if (first) {
            cout << r4 << endl;
            first = false;
        }
        if (seen.count(r4) == 1) break;
        seen.insert(r4);
    }
    cout << last << endl;
    return 0;
}