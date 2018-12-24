#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <tuple>

using namespace std;

enum Attack {
    radiation,
    cold,
    bludgeoning,
    slashing,
    fire
};

struct group {
    int id;
    int team;
    int units;
    int hp;
    Attack attack;
    int power;
    int initiative;
    set<Attack> immunities;
    set<Attack> weaknesses;
};

map<int, group> forces;

int boost = 131;

void init() {
    int id = 0;
    vector<group> immune;
    vector<group> intruders;
    immune = {
        { id++, 0, 3916, 3260, Attack::radiation, 8, 16, {}, {}},
        { id++, 0, 4737, 2664, Attack::slashing, 5, 13, {Attack::radiation, Attack::cold, Attack::bludgeoning }, { } },
        { id++, 0, 272, 10137, Attack::slashing, 331, 10, {}, {}},
        { id++, 0, 92, 2085, Attack::bludgeoning, 225, 1, { Attack::fire }, { } },
        { id++, 0, 126, 11001, Attack::bludgeoning, 717, 8, { Attack::bludgeoning }, { Attack::cold, Attack::fire }},
        { id++, 0,378, 4669, Attack::fire, 117, 17, { Attack::cold, Attack::slashing }, {}},
        { id++, 0,4408, 11172, Attack::bludgeoning, 21, 5, { Attack::slashing}, { Attack::bludgeoning }},
        { id++, 0,905, 11617, Attack::fire, 100, 20, {}, {Attack::fire}},
        { id++, 0,3574, 12385, Attack::radiation, 27, 19, { Attack::radiation}, { Attack::bludgeoning}},
        { id++, 0,8186, 3139, Attack::bludgeoning, 3, 9, { Attack::bludgeoning, Attack::fire}, {}}
    };
    intruders = {
        { id++, 1,273, 26361, Attack::radiation, 172, 18, { Attack::radiation}, { Attack::slashing}},
        { id++, 1,536, 44206, Attack::bludgeoning, 130, 12, {}, { Attack::fire, Attack::cold}},
        { id++, 1,1005, 12555, Attack::radiation, 24, 6, { Attack::fire, Attack::radiation, Attack::bludgeoning}, {}},
        { id++, 1,2381, 29521, Attack::slashing, 23, 4, { Attack::bludgeoning, Attack::radiation}, {}},
        { id++, 1,5162, 54111, Attack::fire, 19, 2, {}, { Attack::radiation}},
        { id++, 1,469, 45035, Attack::radiation, 163, 15, {}, { Attack::fire, Attack::slashing }},
        { id++, 1, 281, 23265, Attack::radiation, 135, 11, { Attack::bludgeoning}, { Attack::slashing }},
        { id++, 1,4350, 46138, Attack::bludgeoning, 18, 14, {}, { Attack::fire }},
        { id++, 1,3139, 48062, Attack::bludgeoning, 28, 3, { Attack::bludgeoning, Attack::slashing, Attack::fire}, { Attack::cold }},
        { id++, 1,9326, 41181, Attack::cold, 8, 7, {}, { Attack::fire, Attack::bludgeoning}}
    };

    for (auto& g : immune) {
        g.power += boost;
        forces[g.id] = g;
    }
    for (auto& g : intruders) forces[g.id] = g;
}

int effective_power(int id) {
    group& g = forces[id];
    return g.units*g.power;
}

bool compare_power(int a, int b) {
    int ap = effective_power(a);
    int bp = effective_power(b);
    if (ap == bp) return forces[a].initiative < forces[b].initiative;
    return ap < bp;
}

vector<int> get_ids() {
    vector<int> out;
    for (auto& p : forces) out.push_back(p.first);
    return out;
}

map<int, int> target_selection() {
    map<int, int> targets;
    map<int, set<int>> enemies;
    for (auto& p : forces) enemies[p.second.team?0:1].insert(p.second.id);
    vector<int> sorted = get_ids();
    sort(sorted.rbegin(), sorted.rend(), compare_power);
    for (int id : sorted) {
        auto& g = forces[id];
        int eid = -1;
        int damage = 0;
        int epower = 0;
        int einit = 0;
        for (int n : enemies[g.team]) {
            auto& e = forces[n];
            if (e.immunities.count(g.attack)) continue;
            int d = effective_power(id);
            if (e.weaknesses.count(g.attack)) d*=2;
            if (d > damage || (d == damage && effective_power(n) > epower) || 
                (d == damage && effective_power(n) == epower && e.initiative > einit)) {
                eid = n;
                damage = d;
                epower = effective_power(n);
                einit = e.initiative;
            }
        }
//        cout << id << " selects " << eid << endl;
        if (damage == 0) eid = -1;
        targets[id] = eid;
        enemies[g.team].erase(eid);
    }
    return targets;
}

pair<int, int> census() {
    pair<int, int> out = { 0, 0};
    for (auto& p : forces) {
        group& g = p.second;
        if (g.team) {
            out.second += g.units;
        } else {
            out.first += g.units;
        }
    }
    return out;
}

void dump() {
    for (auto& [id, g] : forces) {
        cout << g.id << " " << g.team << " " << g.units << endl;
    }
}

void melee(map<int,int>& targets) {
    vector<int> order = get_ids();
    sort(order.begin(), order.end(), [](int a, int b){
        return forces[a].initiative > forces[b].initiative;
    });
  //  dump();
  //  cout << "fight" << endl;
    for (int id : order) {
        if (targets[id] == -1) continue;
        if (forces.count(id) == 0) continue;
        if (forces.count(targets[id]) == 0) continue;
        auto& g = forces[id];
        auto& t = forces[targets[id]];
        int damage = g.power * g.units;
        if (t.immunities.count(g.attack)) continue;
        if (t.weaknesses.count(g.attack)) damage *= 2;
        int lost = damage / t.hp;
        t.units -= lost;
 //       cout << g.id << " hits " << t.id << ": " << damage << " "  << lost << endl;  
        if (t.units <= 0) {
            forces.erase(targets[id]);
        }
    }
}

int fight() {
    pair<int, int> last;
    while (true) {
        auto targets = target_selection();
        melee(targets);
        auto p = census();
        if (p.first == 0) {
            return p.second;
        }
        if (p.second == 0) {
            cout << "good" << endl; 
            return p.first;
        }
        if (p.first == last.first && p.second ==last.second) {
            cout << "stalemate" << endl;
            return -1;
        }
        last = p;
    }
}

int main() {
    init();
    int res = fight();
    cout << res << endl;
}