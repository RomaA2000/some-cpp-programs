//
// Created by Роман Агеев on 31.05.2020.
//
#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  constexpr uint64_t max = 20000001, root = 4473;
  std::vector<bool> mask(max + 1, true);
  mask[0] = mask[1] = false;
  for (uint64_t i = 2; i < max; ++i) {
    if (mask[(size_t)i] && i <= root) {
      for (uint64_t j = i * i; j < max; j += i) {
        mask[(size_t)j] = false;
      }
    }
  }
  uint64_t n = 0;
  std::cin >> n;
  uint64_t m = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> m;
    if (mask[(size_t)m]) {
      std::cout << "YES" << "\n";
    } else {
      std::cout << "NO" << "\n";
    }
  }
}
