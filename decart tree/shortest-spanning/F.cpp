//
// Created by Роман Агеев on 2019-10-27.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <functional>

using std::vector;
using std::pair;
using std::cin;
using std::set;
using std::endl;
using std::cout;
using std::queue;
using std::priority_queue;

template<typename T>
using graph =  vector<vector<T>>;
using matrix = graph<size_t>;

struct edge {
  size_t begin;
  size_t end;
  int64_t cost;
};

using list_edge = vector<edge>;

namespace {
const int64_t INFINITY = 1000000000;
}

void repeat(std::function<void()> const &f, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    f();
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0;
  std::vector<edge> edge_list;
  edge_list.resize(n + 1);
  cin >> n;
  int64_t h = 0;
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      cin >> h;
      if (h != INFINITY) {
        edge_list.push_back({i, j, h});
      }
    }
  }
  std::vector<int64_t> length(n, INFINITY);
  std::vector<size_t> from_where(n);
  length[0] = 0;
  int64_t next = -1;
  repeat([&next, &edge_list, &from_where, &length]() {
    next = -1;
    for (auto i : edge_list) {
      int64_t cost = i.cost + length[i.begin];
      if (cost < length[i.end]) {
        next = (int64_t) i.end;
        from_where[(size_t)next] = i.begin;
        length[(size_t)next] = std::max(-INFINITY, cost);
      }
    }
  }, n);
  std::vector<size_t> cycle;
  if (next == -1) {
    cout << "NO";
  } else {
    auto new_next = (size_t) next;
    cout << "YES" << endl;
    repeat([&new_next, &from_where]() {
      new_next = from_where[new_next];
    }, n);
    size_t now = new_next;
    size_t last = now;
    while ((cycle.empty()) || (new_next != now)) {
      cycle.push_back(now);
      now = from_where[now];
    }
    cycle.push_back(last);
    std::cout << cycle.size() << endl;
    std::reverse(cycle.begin(), cycle.end());
    for (size_t i : cycle) {
      cout << i + 1 << ' ';
    }
  }
  cout << endl;
  return 0;
}
