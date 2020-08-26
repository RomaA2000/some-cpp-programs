//
// Created by Роман Агеев on 07.06.2020.
//
#include <cstdint>
#include <iostream>

template<typename T>
uint64_t log_func(uint64_t start, uint64_t x, uint64_t y, uint64_t p, T f) {
  uint64_t ans = start;
  while (y != 0) {
    if (y % 2 == 1) {
      ans = f(ans, x, p);
    }
    y >>= 1u;
    x = f(x, x, p);
  }
  return ans;
}

uint64_t mod_mul(uint64_t x, uint64_t y, uint64_t p) {
  return log_func(0, x, y, p, [](uint64_t a, uint64_t b, uint64_t c) { return (a + b) % c; });
}

uint64_t mod_pow(uint64_t x, uint64_t y, uint64_t p) {
  return log_func(1, x, y, p, mod_mul);
}

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

uint64_t reverse(uint64_t a, uint64_t b) {
  int64_t x = 0, y = 0;
  gcd(a, b, x, y);
  x = (x + b) % b;
  return (uint64_t) x;
}

int main() {
  uint64_t rsa_n = 0, rsa_e = 0, rsa_c = 0;
  std::cin >> rsa_n >> rsa_e >> rsa_c;
  uint64_t q = 0;
  uint64_t p = 0;
  for (uint64_t i = 2; i * i <= rsa_n; ++i) {
    if (rsa_n % i == 0) {
      p = i;
      q = rsa_n / i;
      break;
    }
  }
  uint64_t d = reverse(rsa_e, (p - 1) * (q - 1));
  std::cout << mod_pow(rsa_c, d, rsa_n);
}



