//
// Created by Роман Агеев on 2019-10-23.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>

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
using edge = pair<size_t, size_t>;
using node = pair<size_t, size_t>;

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0;
  size_t m = 0;
  cin >> n >> m;
  graph<edge> graph;
  vector<size_t> length(n, UINT32_MAX);
  length[0] = 0;
  set<node> queue;
  graph.resize(n);
  size_t a = 0, b = 0;
  size_t c = 0;
  for (size_t i = 0; i < m; ++i) {
    cin >> a >> b >> c;
    --a;
    --b;
    graph[a].emplace_back(b, c);
    graph[b].emplace_back(a, c);
  }
  queue.emplace(length[0], 0);
  while (!queue.empty()) {
    auto now = queue.begin()->second;
    queue.erase(queue.begin());
    for (auto & [f, s] : graph[now]) {
      if ((length[f] > length[now] + s)) {
        queue.erase({length[f], f});
        length[f] = length[now] + s;
        queue.emplace(length[f], f);
      }
    }
  }
  for (size_t i : length) {
    cout << i << " ";
  }
  cout << endl;
}
