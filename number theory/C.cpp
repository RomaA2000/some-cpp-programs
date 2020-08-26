//
// Created by Роман Агеев on 03.06.2020.
//
#include<iostream>
#include <array>
#include <cmath>
#include <cstring>

constexpr uint64_t max_number = 100000;
constexpr uint64_t block_size = 10000;

uint64_t primes[max_number];
bool number_prime[max_number];
bool block[block_size];

int main() {
  uint64_t count = 0;
  uint64_t n = 0;
  int32_t x = 0;
  std::cin >> n;
  std::cin >> x;
  uint64_t root = ((uint64_t)sqrt((double)n)) + 1;
  for (uint64_t i = 2; i < root; ++i) {
    if (!number_prime[(size_t)i]) {
      primes[(size_t)count] = i;
      ++count;
      if (i * i < root) {
        for (uint64_t j = i * i; j < root; j += i) {
          number_prime[(size_t)j] = true;
        }
      }
    }
  }
  int32_t h = 0;
  for (uint64_t _size = 0, max_size = (n / block_size); _size <= max_size; ++_size) {
    std::memset(block, 0, sizeof(block));
    uint64_t first = _size * block_size;
    uint64_t start_idx = 0;
    uint64_t j = 0;
    for (uint64_t i = 0; i < count; ++i) {
      start_idx = (first + primes[(size_t)i] - 1) / primes[(size_t)i];
      for (j = std::max<uint64_t>(start_idx, 2u) * primes[(size_t)i] - first;
      j < block_size; j += primes[(size_t)i]) {
        block[(size_t)j] = true;
      }
    }
    if (_size == 0) {
      block[0] = block[1] = true;
    }
    for (uint64_t i = 0; i < block_size && first + i <= n; ++i) {
      if (!block[(size_t)i]) {
        h = (h * x) + int32_t(first) + int32_t(i);
      }
    }
  }
  std::cout << h;
}
