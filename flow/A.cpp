//
// Created by Роман Агеев on 20.12.2019.
//
#include <vector>
#include <iostream>
#include <functional>
#include <numeric>
template<typename T>
using graph = std::vector<std::vector<T>>;

int main() {
  size_t nl = 0;
  size_t nr = 0;
  size_t m = 0;
  while (std::cin >> nl >> nr >> m) {
    graph<size_t> graph;
    graph.resize(nr);
    size_t a = 0;
    size_t b = 0;
    for (size_t i = 0; i < m; ++i) {
      std::cin >> a >> b;
      --a;
      --b;
      std::swap(a, b);
      graph[a].push_back(b);
    }
    auto int_for = [&]
        (size_t now_v, std::function<bool(size_t)> const &f, std::vector<int64_t> &l,
         std::vector<int64_t> &r) {
      for (auto i : graph[now_v])
        if (f(i)) {
          l[i] = (int64_t) now_v;
          r[now_v] = (int64_t) i;
          return true;
        }
      return false;
    };

    using m_p = std::pair<std::vector<int64_t>, std::vector<int64_t>>;

    std::function<bool(size_t, std::vector<int64_t> &,
                       std::vector<int64_t> &, std::vector<bool> &)>
        kuhn_test = [&](size_t v, std::vector<int64_t> &l,
                        std::vector<int64_t> &r, std::vector<bool> &was) -> bool {
      if (!was[v]) {
        was[v] = true;
        if (int_for(v, [&](size_t e) -> bool {
          return l[e] == -1;
        }, l, r)) {
          return true;
        }
        return int_for(v, [&](size_t e) -> bool {
          return kuhn_test((size_t)l[e], l, r, was);
        }, l, r);
      }
      return false;
    };
    auto kuhn = [&]() -> m_p {
      bool cont = true;
      std::vector<int64_t> matching_r(nr, -1);
      std::vector<int64_t> matching_l(nl, -1);
      while (cont) {
        cont = false;
        std::vector<bool> was(nr, false);
        for (size_t i = 0; (i < nr); ++i) {
          cont |= (matching_r[i] == -1)
              && kuhn_test(i, matching_l, matching_r, was);
        }
      }
      return std::pair(matching_r, matching_l);
    };
    auto r = kuhn();
    std::function<void(size_t, std::vector<bool> &, std::vector<bool> &, m_p &)> dfs_2;
    auto dfs_1 = [&](size_t v, std::vector<bool> &ans, std::vector<bool> &was, m_p &match) {
      ans[v] = true;
      for (auto i : graph[v]) {
        if (!was[i]) {
          dfs_2(i, ans, was, match);
        }
      }
    };
    dfs_2 = [&](size_t v, std::vector<bool> &ans, std::vector<bool> &was, m_p &match) {
      was[v] = true;
      dfs_1((size_t) match.second[v], ans, was, match);
    };
    auto holl = [&](m_p &match) -> std::vector<bool> {
      std::vector<bool> ans(nr, false);
      std::vector<bool> was(nl, false);
      for (size_t i = 0; i < nr; ++i) {
        if (match.first[i] == -1) {
          dfs_1(i, ans, was, match);
          break;
        }
      }
      return ans;
    };
    auto h = holl(r);
    std::vector<size_t> ans;
    for (size_t i = 0; i < nr; ++i) {
      if (h[i]) {
        ans.push_back(i);
      }
    }
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](auto i) {
      std::cout << i + 1 << ' ';
    });
    std::cout << std::endl << std::endl;
  }
}
/*
5 1 5
1 1
2 1
3 1
4 1
5 1
 */
