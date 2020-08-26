//
// Created by Роман Агеев on 2019-10-29.
//
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <tclDecls.h>

using std::pair;
using std::cin;
using std::endl;
using std::cout;
using std::array;
using std::vector;

namespace {
const int64_t INFINITY = INT64_MAX;
}

template<typename T>
using graph =  vector<vector<T>>;

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

int64_t find_mst(graph<pair<size_t, int64_t>> const & graph,
    vector<edge> edge_list, size_t start) {
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0, m = 0;
  cin >> n >> m;
  size_t a = 0, b = 0;
  int64_t c = 0;
  size_t start = 0;

  vector<edge> edge_list;
  edge_list.reserve(m);
  graph<pair<size_t, int64_t>> graph(n);
  for (size_t i = 0; i < m; ++i) {
    cin >> a >> b >> c;
    --a;
    --b;
    edge_list.emplace_back(a, b, c);
    graph[a].emplace_back(b, c);
  }

  vector<bool> used(n, false);
  std::function<void(size_t)> dfs;
  dfs = [&used, &graph, &dfs](size_t new_v) -> void {
    if (!used[new_v]) {
      used[new_v] = true;
      for (auto & [i, j] : graph[new_v]) {
        if (!used[i]) {
          dfs(i);
        }
      }
    }
  };
  dfs(start);
  bool has_ans = true;
  for (auto i : used) {
    if (!i) {
      has_ans = false;
      break;
    }
  }
  if (has_ans) {
    cout << "YES" << endl;
    cout << find_mst(graph,  edge_list, start) << endl;
  } else {
    cout << "NO" << endl;
  }

  return 0;
}

