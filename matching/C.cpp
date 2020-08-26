#include <iostream>
#include <vector>
#include <functional>
#include <unordered_set>
#include <iterator>
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
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n = 0, m = 0;
  cin >> m >> n;
  graph<size_t> g(m + n);
  size_t a = 0, k = 0;
  for (size_t i = 0; i < m; ++i) {
    cin >> k;
    for (size_t j = 0; j < k; ++j) {
      cin >> a;
      --a;
      g[i].push_back(a + m);
    }
  }
  unordered_set<size_t> unreachable;
  unordered_set<size_t> lp, rp;
  vector<pair<size_t, size_t>> matching;
  matching.reserve(m);
  for (size_t i = 0; i < m; ++i) {
    cin >> a;
    if (a != 0) {
      --a;
      g[m + a].emplace_back(i);
    } else {
      unreachable.insert(i);
    }
  }
  vector<bool> first_part(m, true);
  vector<bool> second_part(n, false);
  vector<bool> was(m + n, false);
  std::function<void(size_t)> kuhn = [&was, &g, &kuhn, &m, &first_part, &second_part](size_t v) {
    if (!was[v]) {
      was[v] = true;
      if (v < m) {
        first_part[v] = false;
      } else {
        second_part[v - m] = true;
      }
      for (size_t i : g[v]) {
        if (!was[i]) {
          kuhn(i);
        }
      }
    }
  };
  for (size_t v : unreachable) {
    if (!was[v]) {
      kuhn(v);
    }
  }
  vector<size_t> f_p;
  vector<size_t> s_p;
  std::function<void(vector<bool> const & , std::back_insert_iterator<vector<size_t> > p)>
      inserter = []
      (vector<bool> const & s, std::back_insert_iterator<vector<size_t>> p)  {
    for (size_t i = 0; i < s.size(); ++i) {
      if (s[i]) {
        p = i;
      }
    }
  };
  inserter(first_part, std::back_insert_iterator(f_p));
  inserter(second_part, std::back_insert_iterator(s_p));
  cout << f_p.size() + s_p.size() << endl;
  std::function<void(vector<size_t> const &)> cout_part = [] (vector<size_t> const & a) {
    cout << a.size() << " ";
    for (size_t i = 0; i < a.size(); ++i) {
      cout << a[i] + 1;
      if (i < a.size() - 1) {
        cout << " ";
      } else {
        cout << endl;
      }
    }
    if (a.empty()) {
      cout << endl;
    }
  };
  cout_part(f_p);
  cout_part(s_p);
  return 0;
}
