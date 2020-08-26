//
// Created by Роман Агеев on 22.02.2020.
//
#include <iostream>
#include <vector>

int main() {
  std::vector<uint64_t> hash, pow;
  std::string in;
  std::cin >> in;
  constexpr size_t a = 239;
  constexpr size_t b = 11;
  hash.resize(in.size() + 1, 0);
  pow.resize(in.size(), 1);
  for (size_t i = 1; i < pow.size(); ++i) {
    pow[i] = a * pow[i - 1];
  }
  for (size_t i = 1; i < hash.size(); ++i) {
    hash[i] = ((uint64_t) (in[i - 1] - 'a') + b) * pow[i - 1];
  }
  for (size_t i = 1; i < hash.size(); ++i) {
    hash[i] += hash[i - 1];
  }
  size_t num = 0;
  std::cin >> num;
  size_t a1 = 0, a2 = 0, b1 = 0, b2 = 0;
  for (size_t i = 0; i < num; ++i) {
    std::cin >> a1 >> a2 >> b1 >> b2;
    if (b1 < a1) {
      std::swap(a1, b1);
      std::swap(a2, b2);
    }
    size_t len1 = a2 - a1;
    size_t len2 = b2 - b1;
    if (len1 != len2) {
      std::cout << "No" << "\n";
    } else {
      uint64_t f = hash[a2] - hash[a1 - 1];
      uint64_t s = hash[b2] - hash[b1 - 1];
      f *= pow[b1 - a1];
      std::cout << ((f == s) ? "Yes" : "No") << "\n";
    }
  }
  return 0;
}
