//
// Created by Роман Агеев on 2019-11-15.
//
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
  size_t t = 0;
  cin >> t;
  while (t--) {
    size_t n = 0, m = 0;
    cin >> m >> n;
    graph<bool> g(m, vector<bool>(n, true));
    vector<bool> unreachable(m, true);
    vector<int64_t> matching(n, -1);
    vector<bool> first_part(m, false);
    vector<bool> second_part(n, true);
    vector<bool> was;

    size_t a;
    for (size_t i = 0; i < m; ++i) {
      while ((cin >> a) && (a != 0)) {
        --a;
        g[i][a] = false;
      }
    }

    std::function<bool(size_t)> kuhn = [&was, &g, &matching, &kuhn](size_t v) -> bool {
      if (!was[v]) {
        was[v] = true;
        for (size_t i = 0; i < g[v].size(); ++i) {
          if (g[v][i]) {
            if (matching[i] == -1 || kuhn((size_t) matching[i])) {
              matching[i] = (size_t) v;
              return true;
            }
          }
        }
      }
      return false;
    };

    for (size_t i = 0; i < m; ++i) {
      was.assign(m, false);
      kuhn(i);
    }

    for (size_t i = 0; i < n; ++i) {
      if (matching[i] != -1) {
        unreachable[(size_t) matching[i]] = false;
      }
    }

    graph<size_t> match(m + n, vector<size_t>());

    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        if (g[i][j]) {
          if (i != matching[j]) {
            match[i].push_back(m + j);
          } else {
            match[m + j].push_back(i);
          }
        }
      }
    }

    std::function<void(size_t)> s_dfs = [&was, &match, &s_dfs,
                                         &m, &first_part, &second_part](size_t v) {
      if (!was[v]) {
        was[v] = true;
        if (v < m) {
          first_part[v] = true;
        } else {
          second_part[v - m] = false;
        }
        for (size_t i : match[v]) {
          if (!was[i]) {
            s_dfs(i);
          }
        }
      }
    };

    was.clear();
    was.resize(n + m, false);

    for (size_t i = 0; i < unreachable.size(); ++i) {
      if (unreachable[i]) {
        if (!was[i]) {
          s_dfs(i);
        }
      }
    }

    vector<size_t> f_p;
    vector<size_t> s_p;
    std::function<void(vector<bool> const &,
        std::back_insert_iterator<vector<size_t> > p)>
        inserter = []
        (vector<bool> const &s, std::back_insert_iterator<vector<size_t>> p) {
      for (size_t i = 0; i < s.size(); ++i) {
        if (s[i]) {
          p = i;
        }
      }
    };
    inserter(first_part, std::back_insert_iterator(f_p));
    inserter(second_part, std::back_insert_iterator(s_p));
    cout << f_p.size() + s_p.size() << endl;
    cout << f_p.size() << " " << s_p.size() << endl;
    std::function<void(vector<size_t> const &)> cout_part = [](vector<size_t> const &a) {
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
  }
  return 0;
}
