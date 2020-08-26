//
// Created by Роман Агеев on 2019-10-20.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>

using std::vector;
using std::map;
using std::cin;
using std::set;
using std::endl;
using std::cout;
using std::queue;

template<typename T>
using graph =  vector<vector<T>>;

template <typename T>
int64_t get_path(size_t start, size_t finish, graph<T> graph) {
  T max = std::numeric_limits<T>::max();
  vector<bool> used(graph.size());
  vector<T> length(graph.size(), max);
  length[start] = 0;
  for (size_t i = 0; i < graph.size(); ++i) {
    size_t now = 0;
    bool was = false;
    for (size_t j = 0; j < graph.size(); ++j) {
      if (((!was) || (length[now] > length[j])) && (!used[j])) {
        now = j;
        was = true;
      }
    }
    used[now] = true;
    if ((now != finish) && (length[now] != max)) {
      for (size_t j = 0; j < graph.size(); ++j) {
        if ((graph[now][j] != -1) && (length[j] > length[now] + graph[now][j])) {
          length[j] = length[now] + graph[now][j];
        }
      }
    }
  }
  return length[finish] == max ? -1 : length[finish];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0;
  size_t s = 0;
  size_t f = 0;
  cin >> n >> s >> f;
  --s;
  --f;
  graph<int64_t> graph(n, vector<int64_t>(n));
  for (auto & i : graph) {
    for (auto & j : i) {
      cin >> j;
    }
  }
  auto ans = get_path(s, f, graph);
  cout << ans;
  return 0;
}


