#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <string>
#include <set>
#include <numeric>

using namespace std;
template <class T> using chart = vector<vector<T>>;
using point = pair<int, int>;

chart<int> area;

struct actor {
    int x;
    int y;
    int id;
    int team;
    int hp;
    bool dead;

    void move(int nx, int ny) {
        if (area[ny][nx] != 0) cout << "Illegal move " << id << endl;
        area[ny][nx] = id;
        area[y][x] = 0;
        x = nx;
        y = ny;
    }

    void hit() {
    //    cout << id << " hit" << endl;
        hp = hp-(team==0? 3:25);
        if (hp <= 0) {
            dead = true;
            area[y][x] = 0;
        }
    }
};

bool compare_actors(actor& a, actor& b) {
    if (a.y < b.y) return true;
    if (a.y == b.y && a.x < b.x) return true;
    return false;
}

vector<actor> actors;


void print(actor& a) {
    cout << a.id << ": " << a.x << "," << a.y << " " << a.hp <<
         " " << (a.team == 0 ? "Elf" : "Goblin") << endl;
}

void dump_actors() {
    for (auto& a : actors) {
        print(a);
    }
}
template <class T> actor& pick_actor(T& v) {
    sort(v.begin(), v.end(), compare_actors);
    return v[0];
}

set<point> get_neighbors(int x, int y) {
    set<point> out;
    if (y > 0 && area[y-1][x] < INT_MAX) out.insert({ x, y - 1 });
    if (y < area.size()-1 && area[y+1][x] < INT_MAX) out.insert({ x, y + 1 });
    if (x > 0&& area[y][x-1] < INT_MAX) out.insert({ x - 1, y });
    if (x < area[0].size()-1&& area[y][x+1] < INT_MAX) out.insert({ x+1, y});
    return out;
}

set<point> get_free_neighbors(int x, int y) {
    set<point> out;
    if (y > 0 && !area[y-1][x]) out.insert({ x, y - 1 });
    if (y < area.size()-1 && !area[y+1][x]) out.insert({ x, y + 1 });
    if (x > 0 && !area[y][x-1]) out.insert({ x - 1, y });
    if (x < area[0].size()-1 && !area[y][x+1]) out.insert({ x+1, y});
    return out;
}

chart<int> make_distance_chart(int x, int y) {
    chart<int> out(area);
    out[y][x] = 1;
    int d = 2;
    set<point> next = get_neighbors(x, y);
    while(next.size() > 0) {
        set<point> todo;
        for (auto& p : next) {
            if (!out[p.second][p.first]) {
                out[p.second][p.first] = d;
                auto v = get_free_neighbors(p.first, p.second);
                todo.insert(v.begin(), v.end());
            }
        }
        next = todo;
        d++;
    }
    return out;
}

set<point> get_targets(int team) {
    set<point> targets;
    for (auto& a : actors) {
        if (a.team == team && !a.dead) {
            auto v = get_neighbors(a.x, a.y);
            targets.insert(v.begin(), v.end());
        }
    }
    return targets;
}

int get_point(const chart<int>& m, const point& p) {
    return m[p.second][p.first];
}

actor& get_actor_id(int id) {
    auto it = find_if(actors.begin(), actors.end(), [id](auto& a) { return a.id == id; });
    if (it == actors.end()) cout << "failed to get " << id << endl;
    return *it;
}

void dump_chart(const chart<int>& m) {
    for (auto& r : m) {
        for (int i : r) {
            string s = (i == INT_MAX) ? "#" : i > 2000
                         ? "E" : i > 1000
                             ? "G" : i == 0 || i > 9
                                 ? "." : string(1, i+0x30);
            cout << s;
        }
        cout << endl;
    }
}

bool in_range(actor& a) {
    for (auto& p : get_neighbors(a.x, a.y)) {
        int i = get_point(area, p);
        if (i > 1000 && i < 3000) {
            auto b = get_actor_id(i);
            if (b.team != a.team) return true;
        }
    }
    return false;
}

void attack(actor& a) {
//    cout << a.id << " attack" << endl;
    vector<actor> attackable;
    for (auto& p : get_neighbors(a.x, a.y)) {
        int i = get_point(area, p);
        if (i > 1000 && i < 3000) {
            auto b = get_actor_id(i);
            if (b.team != a.team) attackable.push_back(b);
        }
    }
    sort(attackable.begin(), attackable.end(), [](actor& a, actor& b) {
        if (a.hp == b.hp) return compare_actors(a, b); else return a.hp < b.hp;
    });
    int target = attackable[0].id;
    auto& t = get_actor_id(target);
    t.hit();
}

int get_cost(const chart<int>& m, const point& p) {
    int c = get_point(m, p);
    if (c == 0 || c > 1000) return INT_MAX;
    return c;
}

void move(actor& a) {
//    cout << a.id << " move" << endl;
    chart<int> dists = make_distance_chart(a.x, a.y);
    set<point> targets = get_targets(a.team == 0?1:0);
    auto best = min_element(targets.begin(), targets.end(), [&dists](const point& a, const point& b) {
        int m = get_cost(dists, a);
        int n = get_cost(dists, b);
        if (m < n) return true;
        if (m == n && a.second < b.second) return true;
        if (m == n && a.second == b.second && a.first < b.first) return true;
        return false;
    });
    if (best == targets.end() || get_cost(dists, *best) > 1000) return;
    auto t = get_free_neighbors(a.x, a.y);
    chart<int> revs = make_distance_chart(best->first, best->second);

    if (t.size() == 0) return;
    point choice = *t.begin();
//    dump_chart(revs);
    int cost = get_cost(revs, choice);
    for (auto it = t.begin(); it != t.end(); it++) {
        int c = get_cost(revs, *it);
//        cout << it->first << " " << it->second << " " << c << endl;
        if (c<cost || (c == cost && it->second < choice.second) ||
            (c== cost && it->second == choice.second && it->first < choice.first)) {
            cost = c;
            choice = *it;
        }
    }
//    cout << choice.first << "," << choice.second << " " << a.id << endl;
    a.move(choice.first, choice.second);
    if (in_range(a)) attack(a);
}

void do_round(actor& a) {
    if (a.dead) return;
    if (in_range(a)) {
        attack(a);
    } else {
        move(a);
    }
}

bool run_round() {
    sort(actors.begin(), actors.end(), compare_actors);
    for (auto& a : actors) {
        do_round(a);
    }
//    dump_chart(area);
    auto it = actors.begin();
    int e = 0; int g = 0;
    while(it != actors.end()) {
        if (it->dead) {
            it = actors.erase(it);
        } else {
            if (it->team == 0) e++; else g++;
            it++;
        }
    }
//    dump_actors();
    return e == 0 || g == 0;
}


void read_map() {
    fstream input("data/input15.txt");
    //fstream input("other/test15.txt");
    int x=0, y=0, gid=1000, eid = 2000;
    vector<int> current;
    while(!input.eof()) {
        int c = input.get();
        switch(c) {
            case '#':
                current.push_back(INT_MAX);
                break;
            case '.':
                current.push_back(0);
                break;
            case 'G':
                actors.push_back({ x, y, ++gid, 1, 200, false});
                current.push_back(gid);
                break;
            case 'E':
                actors.push_back({ x, y, ++eid, 0, 200, false});
                current.push_back(eid);
                break;
            case '\n':
                area.push_back(current);
                current.clear();
                y++;
                break;
            case -1:
                break;
            default:
                cout << "ERROR " << c << endl;
        }
        x = current.size();
    }
}

int main() {
    read_map();
    int n = 0;
    dump_chart(area);
    while(!run_round()) {
        n++;
        cout << "Round " << n << endl;
        // if (n == 10) break;
    }
    int hp = 0;
    for (auto& a : actors) {
        hp += a.hp;
    }
    cout << n << " " << hp << " " << n*hp << endl;
    dump_actors();
    cout << actors.size() << endl;
}