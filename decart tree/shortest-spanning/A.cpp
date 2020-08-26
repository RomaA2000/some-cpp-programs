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

template<typename T>
vector<T> shortest_path(T start, graph<T> const &graph) {
  T max = std::numeric_limits<T>::max();
  vector<T> ans(graph.size(), max);
  ans[start] = 0;
  queue<size_t> q;
  q.push(start);
  while (!q.empty()) {
    T now = q.front();
    q.pop();
    for (T const &v : graph[now]) {
      if (ans[v] > ans[now] + 1) {
        ans[v] = ans[now] + 1;
        q.push(v);
      }
    }
  }
  return ans;
}

int main() {
  size_t n = 0;
  size_t m = 0;
  cin >> n >> m;
  size_t a = 0;
  size_t b = 0;
  graph<size_t> graph;
  graph.resize(n);
  for (size_t i = 0; i < m; ++i) {
    cin >> a >> b;
    --a;
    --b;
    graph[a].push_back(b);
    graph[b].push_back(a);
  }
  auto ans = shortest_path((size_t) 0, graph);
  for (size_t i : ans) {
    cout << i << " ";
  }
  return 0;
}
