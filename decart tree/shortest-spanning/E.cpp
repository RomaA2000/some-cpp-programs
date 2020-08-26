//
// Created by Роман Агеев on 2019-10-23.
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

static const int64_t INFINITY = INT64_MAX / 4;

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
  size_t m = 0;
  size_t start = 0;
  cin >> n >> m >> start;
  --start;
  graph<size_t> graph(n);
  vector<int64_t> length(n, INFINITY);
  length[start] = 0;
  list_edge list_edge(m);
  size_t a = 0, b = 0;
  int64_t c = 0;
  for (size_t i = 0; i < m; ++i) {
    cin >> a >> b >> c;
    --a;
    --b;
    graph[a].push_back(b);
    list_edge[i] = {a, b, c};
  }
  repeat([&list_edge, &length]() {
    for (auto i : list_edge) {
      if (length[i.begin] < INFINITY) {
        if (length[i.begin] + i.cost < length[i.end]) {
          length[i.end] = std::max(-INFINITY, length[i.begin] + i.cost);
        }
      }
    }
  }, n - 1);

  vector<bool> used(n, false);

  std::function<void(size_t)> dfs = [&used, &graph, &dfs] (size_t new_v) -> void {
    if (!used[new_v]) {
      used[new_v] = true;
      for (size_t i : graph[new_v]) {
        if (!used[i]) {
          dfs(i);
        }
      }
    }
  };

  vector<size_t> signed_path;
  for (auto i : list_edge) {
    if ((INFINITY > length[i.begin]) && (length[i.end] > length[i.begin] + i.cost)) {
      signed_path.push_back(i.begin);
    }
  }

  for (size_t i : signed_path) {
    dfs(i);
  }

  for (size_t i = 0; i < n; ++i) {
    if (length[i] == INFINITY) {
      cout << '*' << endl;
    } else if (used[i]) {
      cout << '-' << endl;
    }  else {
      cout << length[i] << endl;
    }
  }
}
