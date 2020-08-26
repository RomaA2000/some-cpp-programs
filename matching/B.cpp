//
// Created by Роман Агеев on 2019-11-11.
//

#include <iostream>
#include <vector>
#include <functional>

using std::cin;
using std::vector;
using std::cout;
using std::endl;

template<typename T>
using graph = vector<vector<T>>;

template<typename T>
using edge_list = vector<T>;

int main() {
  size_t n = 0;
  size_t k = 0;
  cin >> n >> k;
  graph<size_t> g(n);
  size_t a = 0, b = 0;
  for (size_t i = 0; i < k; ++i) {
    cin >> a >> b;
    --a;
    --b;
    g[a].push_back(b);
  }
  size_t ans_inc = n;
  vector<int> matching(n, -1);
  vector<bool> was;
  std::function<bool(size_t)> kuhn = [&was, &g, &matching, &kuhn](size_t v) -> bool {
    if (!was[v]) {
      was[v] = true;
      for (size_t i : g[v]) {
        if (matching[i] == -1 || kuhn((size_t) matching[i])) {
          matching[i] = (size_t) v;
          return true;
        }
      }
    }
    return false;
  };

  for (size_t v = 0; v < n; ++v) {
    was.assign(n, false);
    kuhn(v);
  }
  for (int i : matching) {
    if (i != -1) {
      g[i].clear();
      --ans_inc;
    }
  }
  std::cout << ans_inc << endl;
  return 0;
}
