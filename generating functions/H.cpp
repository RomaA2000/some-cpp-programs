//
// Created by Роман Агеев on 22.04.2020.
//
//a(n, m) = ((-1)^(m))*binomial(n-1-m, m),
#include <iostream>
#include <vector>
#include <future>

class gen_func {
  std::vector<int64_t> data;

  static void make_pos(int64_t &i) {
    while (i < 0) {
      i += mod;
    }
    i %= mod;
  }

  int64_t safe_get(size_t i) const {
    return i >= size() ? 0 : data[i];
  }

 public:
  static const int64_t mod = 998244353;
  gen_func(std::vector<int64_t> data) : data(std::move(data)) {}

  size_t size() const {
    return data.size();
  }

  gen_func &shrink_to_fit() {
    while (!empty() && back() == 0) {
      data.pop_back();
    }
    return *this;
  }

  bool empty() const {
    return data.empty();
  }

  int64_t back() const {
    return data.back();
  }

  gen_func &div_(gen_func const &s, size_t num) {
    std::vector<int64_t> ans(num, 0);
    for (size_t i = 0; i < num; ++i) {
      ans[i] = safe_get(i);
      ans[i] %= mod;
      for (size_t j = 0; j < i; ++j) {
        ans[i] -= (ans[j] * s.safe_get(i - j)) % mod;
        ans[i] %= mod;
      }
      make_pos(ans[i]);
    }
    data = ans;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &, const gen_func &);
};

std::ostream &operator<<(std::ostream &s, gen_func const &in) {
  for (auto i : in.data) {
    s << i % gen_func::mod << std::endl;
  }
  return s;
}

int main() {
  constexpr size_t n = 1001;
  constexpr size_t k = (n >> 1u) + (n % 2);
  int64_t C[n][k];
  for (size_t i = 0; i < k; ++i) {
    C[0][i] = 0;
  }
  for (size_t i = 0; i < n; ++i) {
    C[i][0] = 1;
  }
  for (size_t i = 1; i < n; ++i) {
    for (size_t j = 1; j < k; ++j) {
      C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % gen_func::mod;
    }
  }
  size_t k_read = 0;
  size_t length = 0;
  std::cin >> k_read >> length;
  size_t p_num = k_read - 1;
  std::vector<int64_t> P;
  P.reserve((p_num >> 1u) + (p_num % 2));
  for (size_t j = 0; j < (p_num >> 1u) + (p_num % 2); ++j) {
    P.push_back(C[p_num - 1 - j][j]);
    if (j % 2 == 1) {
      P.back() *= -1;
    }
  }
  ++p_num;
  std::vector<int64_t> Q;
  Q.reserve((p_num >> 1u) + (p_num % 2));
  for (size_t j = 0; j < (p_num >> 1u) + (p_num % 2); ++j) {
    Q.push_back(C[p_num - 1 - j][j]);
    if (j % 2 == 1) {
      Q.back() *= -1;
    }
  }
  auto f_g_f_p = std::async([&] () {return gen_func(P);});
  auto f_g_f_q = std::async([&] () {return gen_func(Q);});
  std::cout << f_g_f_p.get().div_(f_g_f_q.get(), length);
}