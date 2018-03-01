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
    : sx(_sx), sy(_sy), fx(_fx), fy(_fy), s(_s), f(_f), id(_id)
  {
    dist = abs(fx - sx) + abs(fy - sy);
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
vector<bool> visited;
vector<vector<int>> cars;


void readin(string filename) {
  ifstream fin(filename + ".in");

  fin >> R >> C >> F >> N >> B >> T;

  rides.emplace_back(0, 0, 0, 0, 0, 0);

  for (int i = 0; i < N; i += 1) {
    int sx, sy, fx, fy, s, f;
    fin >> sx >> sy >> fx >> fy >> s >> f;
    rides.emplace_back(sx, sy, fx, fy, s, f, i);
  }

  sort(rides.begin(), rides.end(), [](auto& r1, auto& r2) {
    if (r1.f == r2.f) return r1.s < r2.s;
    return r1.f < r2.f;
  });
  best.resize(N + 1, 0);
  used.resize(N + 1, 0);
  from.resize(N + 1, 0);
  visited.resize(N + 1, 0);
}

inline int dist(int i, int j) {
  return abs(rides[i].fx - rides[j].sx) + abs(rides[i].fy - rides[j].sy);
}

void makeGraph() {
  for (int i = 0; i < N; i += 1) {
    for (int j = i + 1; j <= N; j += 1) {
      // cerr << i << ' ' << rides[j].id << endl;
      // rides[i].print();
      // rides[j].print();
      // worst time
      int total = rides[i].s + rides[i].dist + dist(i, j) + rides[j].dist;

      if (total <= rides[j].f) {
        graph[i].emplace_back(j);
      }
    }
  }
}

int totalBooty = 0;
void findPath() {
  auto comp = [](auto &p1, auto &p2) {
    return best[p1.X] > best[p2.X];
  };
  priority_queue<PII, vector<PII>, decltype(comp)> pq(comp);
  pq.emplace(0, 0);

  while (not pq.empty()) {
    int node = pq.top().X;
    int t = pq.top().Y;
    pq.pop();

    if (visited[node]) continue;
    visited[node] = true;

    for (auto next: graph[node]) {
      if (used[next]) continue;

      int cost = t + dist(node, next);

      if (cost + rides[next].dist <= rides[next].f) {
        int booty = best[node] + rides[next].dist;

        if (cost <= rides[next].s) {
          booty += B;
          cost = max(cost, rides[next].s);
        }

        if (booty > best[next]) {
          best[next] = booty;
          from[next] = node;
          pq.emplace(next, cost + rides[next].dist);
        }
      }
    }
  }

  int bestNode = 0;
  for (int i = 1; i <= N; i += 1) {
    if (best[i] > best[bestNode]) {
      bestNode = i;
    }
  }

  cars.emplace_back();
  for (int i = bestNode; i != 0; i = from[i]) {
    // rides[i].print();
    used[i] = true;
    cars.back().push_back(rides[i].id);
  }

  cout << "Won " << best[bestNode] << " $$$" << endl;
  totalBooty += best[bestNode];
  fill(from.begin(), from.end(), 0);
  fill(best.begin(), best.end(), 0);
  fill(visited.begin(), visited.end(), 0);
  reverse(cars.back().begin(), cars.back().end());
}

void solve() {
  makeGraph();
  cout << "Made graph" << endl;

  for (int i = 0; i < F; i += 1) {
    findPath();
  }
}

void printout(string filename) {
  ofstream fout(filename + ".out");

  for (auto car: cars) {
    fout << car.size() << ' ';
    for (auto r: car) {
      fout << r << ' ';
    }
    fout << '\n';
  }
  cout << "Total booty: " << totalBooty << endl;
}

int main(int argc, char* argv[]) {
  readin(argv[1]);
  cout << "Finished reading" << endl;
  solve();
  printout(argv[1]);

  return 0;
}