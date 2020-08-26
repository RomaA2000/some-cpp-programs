//
// Created by Роман Агеев on 02.01.2020.
//
#include <iostream>
#include <vector>
#include <functional>
#include <future>
#include <algorithm>

template<typename T>
using graph = std::vector<std::vector<T>>;

template<typename T>
void sort(T && f, T && s) {
  std::sort(f, s);
}

int main() {

  freopen("matching.in", "r", stdin);
  freopen("matching.out", "w", stdout);

  size_t n = 0;
  std::cin >> n;
  graph<size_t> g(n);
  std::vector<std::pair<uint64_t, size_t>> weights;
  weights.reserve(n);
  uint64_t w = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> w;
    weights.emplace_back(w, i);
  }
  auto get_sorted = std::async(std::launch::deferred, sort<decltype(weights.begin())>, weights.begin(), weights.end());
  size_t a = 0;
  size_t num = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> num;
    for (size_t j = 0; j < num; ++j) {
      std::cin >> a;
      --a;
      g[i].push_back(a);
    }
  }
  get_sorted.get();
  std::vector<int64_t> matching(n, -1);
  std::vector<bool> was;
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
  for (auto i = weights.crbegin(); i != weights.crend(); ++i) {
    was.assign(n, false);
    kuhn(i->second);
  }
  std::vector<int64_t> ans(n, -1);
  for (size_t i = 0; i < n; ++i) {
    if (matching[i] != -1) {
      ans[(size_t) matching[i]] = i;
    }
  }
  for (auto i : ans) {
    std::cout << i + 1 << ' ';
  }
  return 0;
}

