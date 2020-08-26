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
  size_t n = 0, m = 0;
  size_t k = 0;
  cin >> n >> m >> k;
  graph<size_t> g(std::min(n, m));
  size_t a = 0, b = 0;
  for (size_t i = 0; i < k; ++i) {
    cin >> a >> b;
    --a;
    --b;
    if (n > m) {
      std::swap(a, b);
    }
    g[a].push_back(b);
  }
  size_t r_size = std::max(n, m);
  size_t l_size = std::min(n, m);
  vector<int64_t> matching(r_size, -1);
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

  for (size_t v = 0; v < l_size; ++v) {
    was.assign(l_size, false);
    kuhn(v);
  }

  size_t ans = 0;
  for (int i : matching) {
    if (i != -1) {
      ++ans;
    }
  }
  cout << ans << endl;
  return 0;
}
