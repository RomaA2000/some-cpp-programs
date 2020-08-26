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
using std::pair;
using std::cin;
using std::set;
using std::endl;
using std::cout;
using std::queue;

template<typename T>
using graph =  vector<vector<T>>;

using matrix = graph<size_t>;

void get_all_path(matrix & graph) {
  for (size_t i = 0; i < graph.size(); ++i) {
    for (size_t j = 0; j < graph.size(); ++j) {
      for (size_t k = 0; k < graph.size(); ++k) {
        graph[j][k] = std::min(graph[j][k], graph[j][i] + graph[i][k]);
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0;
  size_t m = 0;
  cin >> n >> m;
  graph<size_t> graph(n, vector<size_t>(n, std::numeric_limits<size_t>::max() / 2));
  size_t a = 0, b = 0, c = 0;
  for (size_t i = 0; i < graph.size(); ++i) {
    graph[i][i] = 0;
  }
  for (size_t i = 0; i < m; ++i) {
    cin >> a >> b >> c;
    --a;
    --b;
    graph[a][b] = c;
  }
  get_all_path(graph);
  for (auto const & i : graph) {
    for (auto const & j : i) {
      cout << j << " ";
    }
    cout << endl;
  }
  return 0;
}



