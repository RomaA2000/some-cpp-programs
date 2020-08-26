//
// Created by Роман Агеев on 03.06.2020.
//
#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  constexpr uint64_t max = 1000001;
  std::vector<uint64_t> _prime(max, 0);
  uint64_t n;
  std::cin >> n;
  for (uint64_t i = 2; i * i < max; ++i) {
    for (uint64_t j = i; j < max; j += i) {
      if (!_prime[(size_t) j]) {
        _prime[(size_t) j] = i;
      }
    }
  }
  auto const & prime = _prime;
  for (uint64_t i = 0; i < n; ++i) {
    size_t j = 0;
    std::cin >> j;
    while (prime[j] != 0) {
      std::cout << prime[j] << " ";
      j /= (size_t)prime[j];
    }
    if (j != 1) {
      std::cout << j;
    }
    std::cout << "\n";
  }
}
