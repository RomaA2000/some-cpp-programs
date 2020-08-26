//
// Created by Роман Агеев on 07.03.2020.
//
#include <iostream>
#include <vector>
#include <string>
#include <functional>

std::vector<std::vector<size_t>> get_matrix(size_t n) {
  std::vector<std::vector<size_t>> ans(n, std::vector<size_t>(n, 0));
  return ans;
}

int main() {
  std::string input;
  std::cin >> input;
  input = '#' + input;
  auto pref = get_matrix(input.size());
  auto ans = get_matrix(input.size());
  for (size_t i = 1u; i < input.size(); ++i) {
    for (size_t j = 1u; j < input.size(); ++j) {
      if (input[i] == input[j] && i != j) {
        pref[i][j] = pref[i - 1u][j - 1u];
        ++pref[i][j];
      }
    }
  }
  size_t tmp;
  for (size_t i = 1u; i < input.size(); ++i) {
    tmp = 0u;
    for (size_t j = i; --j;) {
      if (tmp < pref[i][j]) {
        for (size_t l = tmp; l < pref[i][j]; ++l) {
          ++ans[j - l][i];
        }
        tmp = pref[i][j];
      }
    }
  }
  std::function<void(std::function<size_t(size_t)> const &,
                     std::function<size_t(size_t)> const &, size_t, size_t)>
      resolver = [&](auto f, auto s, size_t c1, size_t c2) {
    for (size_t i = input.size(); --i > c1;) {
      for (size_t j = 1u; j < c2; ++j) {
        if (f(i) < input.size() && s(i) < input.size()
            && i < input.size() && j < input.size()) {
          ans[f(i)][s(j)] += ans[i][j];
        }
      }
    }
  };
  resolver([](size_t i) { return i; }, [](size_t i) { return i + 1; }, 0, input.size() - 1);
  resolver([](size_t i) { return i - 1; }, [](size_t i) { return i; }, 1, input.size());
  size_t num = 0u;
  std::cin >> num;
  ++num;
  size_t l = 0u, r = 0u;
  while (--num) {
    std::cin >> l >> r;
    std::cout << (((r - l + 1u) * (r - l + 2u)) >> 1u) - ans[l][r] << '\n';
  }
  return 0;
}
