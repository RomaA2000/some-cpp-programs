//
// Created by Роман Агеев on 2019-11-15.
//
#include <iostream>
#include <vector>
#include <functional>
#include <unordered_set>
#include <algorithm>

using std::cin;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::unordered_set;

template<typename T>
using graph = vector<vector<T>>;

template<typename T>
using edge_list = vector<T>;

int main() {
  size_t n = 0;
  cin >> n;
  size_t e = 0;
  graph<size_t> g(n, vector<size_t>(n, 0));
  vector<size_t> cycle;
  vector<bool> was(g.size(), false);
  bool no_cycle = false;

  size_t m = 0, a = 0;
  for (size_t i = 0; i < g.size(); ++i) {
    cin >> m;
    for (size_t j = 0; j < m; ++j) {
      cin >> a;
      --a;
      ++g[i][a];
    }
    e += m;
  }

  vector<size_t> degree(g.size(), 0);

  for (size_t i = 0; i < g.size(); ++i) {
    for (size_t j = 0; j < g.size(); ++j) {
      degree[i] += g[i][j];
    }
  }

  size_t counter = 0;
  for (size_t i = 0; i < degree.size(); ++i) {
    if (degree[i] % 2 == 1) {
      ++counter;
    }
  }

  if ((counter != 0) && (counter != 2)) {
    no_cycle = true;
  }

  counter = 0;

  std::function<void(size_t)> dfs = [&was, &g, &counter, &dfs](size_t v) {
    if (!was[v]) {
      was[v] = true;
      for (size_t i = 0; i < g[v].size(); ++i) {
        counter += g[v][i];
        if (g[v][i] > 0) {
          dfs(i);
        }
      }
    }
  };

  dfs(0);

  if (e != counter) {
    no_cycle = true;
  }

  std::function<void(size_t)> gener = [&g, &cycle, &gener](size_t v) {
    for (size_t i = 0; i < g[v].size(); ++i) {
      while (g[v][i] > 0) {
        --g[v][i];
        --g[i][v];
        gener(i);
      }
    }
    cycle.push_back(v);
  };
  gener(0);

  if (no_cycle) {
    cout << -1 << endl;
  } else {
    cout << cycle.size() - 1 << endl;
    for (size_t i = 0; i < cycle.size(); ++i) {
      cout << cycle[i] + 1;
      if (i < cycle.size() - 1) {
        cout << " ";
      } else {
        cout << endl;
      }
    }
  }
}
