//
// Created by Роман Агеев on 2019-10-29.
//
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

using std::pair;
using std::cin;
using std::endl;
using std::cout;
using std::array;
using std::vector;

namespace {
const int64_t INFINITY = INT64_MAX;
}

struct edge {
  size_t begin;
  size_t end;
  int64_t cost;
};



class DSU {
  vector<size_t> classes;
  vector<size_t> len;

 public:
  explicit DSU(size_t n) : classes(n), len(n) {
    for (size_t i = 0; i < n; ++i) {
      classes[i] = i;
    }
  }

  size_t get_class(size_t v) {
    if (classes[v] != v) {
      classes[v] = get_class(classes[v]);
      return classes[v];
    } else {
      return v;
    }
  }

  void connect(size_t a, size_t b) {
    size_t class_a = get_class(a);
    size_t class_b = get_class(b);
    if (len[class_a] <= len[class_b]) {
      classes[class_a] = class_b;
      if (len[class_a] == len[class_b]) {
        ++len[class_a];
      }
    } else {
      classes[class_b] = class_a;
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0, m = 0;
  cin >> n >> m;
  size_t a = 0, b = 0;
  int64_t c = 0;
  vector<edge> edge_list(m);
  for (size_t i = 0; i < m; ++i) {
    cin >> a >> b >> c;
    edge_list[i] = {a - 1, b - 1, c};
  }
  int64_t delta = INFINITY;
  std::sort(edge_list.begin(), edge_list.end(), [] (edge const &a, edge const &b) {
    return a.cost < b.cost;
  });
  for (size_t i = 0; i < std::min(m, m - n + 2); ++i) {
    DSU dsu(n);
    int64_t min_cost = INFINITY;
    int64_t max_cost = -INFINITY;
    size_t count = 0;
    for (size_t j = i; j < m; ++j) {
      auto now = edge_list[j];
      if (dsu.get_class(now.begin) != dsu.get_class(now.end)) {
        dsu.connect(now.begin, now.end);
        min_cost = std::min(min_cost, now.cost);
        max_cost = std::max(max_cost, now.cost);
        ++count;
      }
    }
    if (count + 1 == n) {
      delta = std::min(max_cost - min_cost, delta);
    }
  }
  if (delta == INFINITY) {
    cout << "NO" << endl;
  } else {
    cout << "YES" << endl;
    cout << delta << endl;
  }
}

