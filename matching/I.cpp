//
// Created by Роман Агеев on 2019-11-17.
//
#include <iostream>
#include <vector>
#include <functional>
#include <set>
#include <algorithm>

using std::cin;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::multiset;

template<typename T>
using graph = vector<vector<T>>;

template<typename T>
using edge_list = vector<T>;

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t d = 0, k = 0;
  cin >> d >> k;
  size_t n = 1;
  if (k != 1) {
    for (size_t i = 1; i < k; ++i) {
      n *= d;
    }
    std::function<size_t(size_t)> solver = [n, d] (size_t i) -> size_t {
      return d * (i % (n / d));
    };
    vector<multiset<size_t>> g(n);
    vector<size_t> cycle;
    for (size_t i = 0; i < n; ++i) {
      auto t = solver(i);
      for (size_t j = 0; j < d; ++j) {
        g[i].insert(t + j);
      }
    }
    std::function<void(size_t)> gener = [&g, &cycle, &gener](size_t v) {
      while (!g[v].empty()) {
        auto i = *g[v].begin();
        g[v].erase(g[v].begin());
        gener(i);
      }
      cycle.push_back(v);
    };
    gener(0);
    reverse(cycle.begin(), cycle.end());
    for (size_t i = 0; i + 1 < k; ++i) {
      cout << 0;
    }
    for (size_t i = 1; i < cycle.size(); ++i) {
      cout << cycle[i] % d;
    }
  } else {
    for (size_t i = 0; i < d; ++i) {
      cout << i;
    }
  }
  return 0;
}