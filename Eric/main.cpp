#include <bits/stdc++.h>

using namespace std;

using point = pair<int, int>;

#define x first
#define y second

int ride_start_time(int icar, int iride);
int dist(point a, point b);

int dist(point a, point b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
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

vector<int> car_time;
vector<point> car_position;

int r, c;
int ncars;
int nrides;

int bonus;

int simtime;

bool can_arrive(point p1, int t1, point p2, int t2)
{
    return dist(p1, p2) <= t2 - t1;
}

bool car_can_arrive(int icar, int iride)
{
    return can_arrive(car_position[icar], car_time[icar], rides[iride].p1, rides[iride].earliest());
}

vector<int> get_possible_cars(int iride)
{
    vector<int> good;
    for (int i = 0; i < ncars; ++i) {
        if (car_can_arrive(i, iride))
            good.push_back(i);
            continue;
    }
    return good;
}

int get_best_car(int iride)
{
    // Pick the car which is closest.
    vector<int> possible = get_possible_cars(iride);
    if (possible.empty()) {
        return -1;
    }

    int ibest = -1;
    int best = 0;
    for (auto i : possible) {
        int now = dist(car_position[i], rides[iride].p1);

        if (ibest == -1 || now < best) {
            ibest = i;
            best = now;
        }
    }

    assert(ibest != -1);
    return ibest;
}

int ride_start_time(int icar, int iride)
{
    assert(car_can_arrive(icar, iride));

    int car_arrive = car_time[icar] + dist(car_position[icar], rides[iride].p1);
    int ride_start = max(car_arrive, rides[iride].st);

    return ride_start;
}

void apply_car_chosen(int icar, int iride)
{
    assert(car_can_arrive(icar, iride));

    int ride_start = ride_start_time(icar, iride);
    car_position[icar] = rides[iride].p2;
    car_time[icar] = ride_start + rides[iride].cost();

    rides_for_car[icar].push_back(iride);
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

    car_position.resize(ncars);
    car_time.resize(ncars);
    for (int i = 0; i < ncars; ++i) {
        car_time[i] = 0;
        car_position[i] = {0, 0};
    }

    int nnosat = 0;

    for (int i = 0; i < nrides; ++i) {
        int car = get_best_car(i);
        if (car == -1) {
            nnosat++;
            // cerr << "could not satisfy ride " << i << "\n";
            continue;
        }

        apply_car_chosen(car, i);
    }

    output();
    cerr << "could not satisfy " << nnosat << "\n";


    return 0;
}
