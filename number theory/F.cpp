//
// Created by Роман Агеев on 07.06.2020.
//
#include <cstdint>
#include <iostream>

int64_t gcd(uint64_t a, uint64_t b, int64_t &x, int64_t &y) {
  if (a == 0) {
    x = 0;
    y = 1;
    return b;
  }
  int64_t x1 = 0, y1 = 0;
  int64_t d = gcd(b % a, a, x1, y1);
  x = y1 - (b / a) * x1;
  y = x1;
  return d;
}

int main() {
  uint64_t t = 0;
  std::cin >> t;
  int64_t a = 0, b = 0, n = 0, m = 0;
  for (uint64_t i = 0; i < t; ++i) {
    std::cin >> a >> b >> n >> m;
    int64_t x = 0;
    int64_t y = 0;
    gcd(n, m, x, y);
    int64_t  delta = (b - a);
    int64_t r1 = (delta * ((x % m + m) % m)) % m;
    std::cout << (n * r1 + a + m * n) % (m * n) << std::endl;
  }
}
