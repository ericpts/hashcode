#include <cmath>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

constexpr int MAX_N = 10100;

#define X first
#define Y second
#define MP make_pair
typedef pair<int, int> PII;

struct Ride {
  int sx, sy, fx, fy;
  int s, f;
  int dist;
  int id;

  Ride(int _sx=0, int _sy=0, int _fx=0, int _fy=0, int _s=0, int _f=0, int _id=0)
    : sx(_sx), sy(_sx), fx(_fx), fy(_fy), s(_s), f(_f), id(_id)
  {
    cout << "const" << endl;
    dist = abs(fx - sx) + abs(fy - sy);
    cout << sx << sy << fx << fy << s << f << endl;
    print();
  }
  void print() {
    cout << "Ride:" << endl;
    cout << sx << ' ' << sy << ' ' << fx << ' ' << fy << endl;
    cout << s << ' ' << f << endl;
    cout << dist << endl;
    cout << id << endl << endl;
  }
};

int R, C, F, N, B, T;
vector<Ride> rides;
vector<int> graph[MAX_N];
vector<bool> used;
vector<int> best;
vector<int> from;
vector<vector<int>> cars;


void readin(string filename) {
  ifstream fin(filename + ".in");

  fin >> R >> C >> F >> N >> B >> T;

  // starting node;
  rides.emplace_back(0, 0, 0, 0, 0, 0);

  for (int i = 1; i <= N; i += 1) {
    int sx, sy, fx, fy, s, f;
    fin >> sx >> sy >> fx >> fy >> s >> f;
    cout << sx << sy << fx << fy << s << f << endl;
    rides.emplace_back(sx, sy, fx, fy, s, f, i);
  }

  sort(rides.begin(), rides.end(), [](auto& r1, auto& r2) {
    if (r1.f == r2.f) return r1.s < r2.s;
    return r1.f < r2.f;
  });
  best.resize(N + 1, 0);
  used.resize(N + 1, 0);
  from.resize(N + 1, 0);
}

inline int dist(int i, int j) {
  return abs(rides[i].fx - rides[j].sx) + abs(rides[i].fy - rides[j].sy);
}

void makeGraph() {
  for (int i = 0; i < N; i += 1) {
    for (int j = 1; j <= N; j += 1) {
      cerr << i << ' ' << rides[j].id << endl;
      rides[j].print();
      // worst time
      int total = rides[i].s + rides[i].dist + dist(i, j) + rides[j].dist;
      cerr << dist(i,j) << ' ' << rides[j].dist << endl;
      cerr << total << endl << endl;

      if (total <= rides[i].f) {
        graph[i].emplace_back(j);
      }
    }
  }
}

void findPath() {
  queue<PII> q;
  q.emplace(0, 0);

  while (not q.empty()) {
    int node = q.front().X;
    int t = q.front().Y;
    q.pop();

    cout << "node: " << node << endl;
    cout << "best: " << best[node] << endl;

    for (auto next: graph[node]) {
      if (used[next]) continue;

      int cost = t + dist(node, next);
      cerr << next << ' ' << cost << endl;
      if (cost + rides[next].dist <= rides[next].f) {
        int booty = best[node] + rides[next].dist;

        if (cost <= rides[next].s) {
          booty += B;
          cost = max(cost, rides[next].s);
        }

        if (booty > best[next]) {
          best[next] = booty;
          from[next] = node;
          q.emplace(next, cost + rides[next].dist);
        }
      }
    }
  }

  int bestNode = 0;
  for (int i = 1; i <= N; i += 1) {
    if (best[i] > best[bestNode]) {
      bestNode = i;
    }

    best[i] = 0;
  }

  cars.emplace_back();
  for (int i = bestNode; i != 0; i = from[i]) {
    used[i] = true;
    cars.back().push_back(rides[i].id);
  }

  cout << "Won " << best[bestNode] << " $$$" << endl;
  fill(from.begin(), from.end(), 0);
  fill(best.begin(), best.end(), 0);
  reverse(cars.back().begin(), cars.back().end());
}

void solve() {
  makeGraph();

  for (int i = 0; i < F; i += 1) {
    findPath();
  }
}

void printout(string filename) {
  ofstream fout(filename + ".out");

  for (int i = 0; i < F; i += 1) {
    fout << cars[i].size() << ' ';
    for (auto r: cars[i]) {
      fout << r << ' ';
    }
    fout << '\n';
  }
}

int main(int argc, char* argv[]) {
  readin(argv[1]);
  solve();
  printout(argv[1]);

  return 0;
}