#include <bits/stdc++.h>

using namespace std;

using point = pair<int, int>;

#define x first
#define y second

int dist(point a, point b)
{
    return max(abs(a.x - b.x), abs(a.y - b.y));
}

struct Ride {
    point p1, p2;
    int st, fin;
    int idx;

    bool operator< (const Ride &b) const
    {
        if (st == b.st) {
            return fin < b.fin;
        }
        return st < b.st;
    }

    int cost() const
    {
        return dist(p1, p2);
    }

    int earliest() const
    {
        // Earliest time you can start this ride and still finish it.
        return fin - cost();
    }
};

vector<Ride> rides;

vector<vector<int>> rides_for_car;

int r, c;
int ncars;
int nrides;

int bonus;

int simtime;

bool can_arrive(point p1, int t1, point p2, int t2)
{
    return dist(p1, p2) <= t2 - t1;
}

vector<int> get_possible_cars(int iride)
{
    vector<int> good;
    for (int i = 0; i < ncars; ++i) {

        if (rides_for_car[i].empty()) {
            if (can_arrive({0, 0}, 0, rides[iride].p1, rides[iride].earliest())) {
                good.push_back(i);
            }
            continue;
        }

        assert(!rides_for_car[i].empty());
        point fin = rides[rides_for_car[i].back()].p2;
        int tfin = rides[rides_for_car[i].back()].fin;

        if (can_arrive(fin, tfin, rides[iride].p1, rides[iride].earliest())) {
            good.push_back(i);
            continue;
        }
    }
    return good;
}

int get_best_car(int iride)
{
    vector<int> possible = get_possible_cars(iride);
    random_shuffle(possible.begin(), possible.end());

    if (possible.empty()) {
        return -1;
    }

    return possible.back();
}

void output()
{
    for (int i = 0; i < ncars; ++i) {
        cout << rides_for_car[i].size() << " ";
        for (int r : rides_for_car[i]) {
            cout << r << " ";
        }
        cout << "\n";
    }
}

int main()
{
    srand(time(0));
    cin >> r >> c;
    cin >> ncars;
    cin >> nrides;
    cin >> bonus;
    cin >> simtime;

    rides.resize(nrides);
    rides_for_car.resize(ncars);

    for (int i = 0; i < nrides; ++i) {
        cin >> rides[i].p1.x >> rides[i].p1.y;
        cin >> rides[i].p2.x >> rides[i].p2.y;
        cin >> rides[i].st;
        cin >> rides[i].fin;
        rides[i].idx = i;
    }

    sort(rides.begin(), rides.end());

    for (int i = 0; i < nrides; ++i) {
        int car = get_best_car(i);
        if (car == -1) {
            continue;
        }

        rides_for_car[car].push_back(i);
    }

    output();


    return 0;
}
