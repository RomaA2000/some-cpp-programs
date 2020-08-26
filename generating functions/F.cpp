//
// Created by Роман Агеев on 21.04.2020.
//
#include <iostream>
#include <vector>
#include <future>

class gen_func {
  std::vector<int64_t> data;

  int64_t static safe_get(std::vector<int64_t> v, size_t i) {
    return i >= v.size() ? 0 : v[i];
  }

  int64_t safe_get(size_t i) const {
    return safe_get(data, i);
  }

  static int64_t pow_mod(int64_t i, uint64_t num) {
    if (num == 0) {
      return 1;
    }
    int64_t ans = 0;
    if (num % 2 == 1) {
      ans = i * pow_mod(i, num - 1);
    } else {
      int64_t t = pow_mod(i, num >> 1u);
      ans = t * t;
    }
    ans %= mod;
    return ans;
  }

  static int64_t inverse(int64_t i) {
    return pow_mod(i, mod - 2);
  }

  static void make_pos(int64_t &i) {
    if (i < 0) {
      i += mod;
    }
  }
 public:

  static const uint64_t mod = 1000000000 + 7;

  gen_func(std::vector<int64_t> data) : data(std::move(data)) {}

  gen_func &operator+=(gen_func const &in) {
    size_t max_size = std::max(size(), in.size());
    data.resize(max_size, 0);
    size_t min_size = std::min(size(), in.size());
    for (size_t i = 0; i < min_size; ++i) {
      data[i] += in.data[i];
      data[i] %= mod;
    }
    return *this;
  }

  gen_func &operator*=(gen_func const &in) {
    std::vector<int64_t> ans(size() + in.size());
    for (size_t i = 0; i < size(); ++i) {
      for (size_t j = 0; j < in.size(); ++j) {
        int64_t k = ans[i + j];
        k += (data[i] * in.data[j] % mod);
        k %= mod;
        make_pos(k);
        ans[i + j] = k;
      }
    }
    shrink_to_fit(ans);
    data = ans;
    return *this;
  }

  size_t size() const {
    return data.size();
  }

  static std::vector<int64_t> &shrink_to_fit(std::vector<int64_t> &in) {
    while (!in.empty() && in.back() == 0) {
      in.pop_back();
    }
    return in;
  }

  gen_func &shrink_to_fit() {
    shrink_to_fit(data);
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
      for (size_t j = 0; j < i; ++j) {
        ans[i] -= (ans[j] * s.safe_get(i - j)) % mod;
        make_pos(ans[i]);
      }
      ans[i] /= s.safe_get(0);
    }
    data = ans;
    return *this;
  }

  std::vector<int64_t> get_tmp_prod(std::vector<int64_t> &in, size_t last) const {
    size_t sz = std::min(in.size() + size() + 1, last);
    std::vector<int64_t> ans(sz);
    for (size_t i = 0; i < sz; ++i) {
      for (size_t j = 0; j <= i; ++j) {
        ans[i] += (safe_get(in, j) * safe_get(i - j)) % mod;
        ans[i] %= mod;
        make_pos(ans[i]);
      }
    }
    shrink_to_fit(ans);
    return ans;
  }

  friend std::ostream &operator<<(std::ostream &, const gen_func &);

  gen_func &to_exp_(size_t num) {
    std::vector<int64_t> exp(num, 0);
    exp[0] = 1;
    int64_t c = 0;
    int64_t prod = 1;
    std::vector<int64_t> now(1, 1);
    for (size_t i = 1; i < num; ++i) {
      now = get_tmp_prod(now, num);
      prod *= i;
      prod %= mod;
      c = inverse(prod);
      for (size_t j = 0; j < num; ++j) {
        exp[j] += (c * safe_get(now, j) % mod);
        exp[j] %= mod;
        make_pos(exp[j]);
      }
    }
    data = exp;
    return *this;
  }

  gen_func &to_log_(size_t num) {
    std::vector<int64_t> log(num, 0);
    int64_t c = 0;
    int64_t prod = mod - 1;
    std::vector<int64_t> now(1, 1);
    for (size_t i = 1; i < num; ++i) {
      now = get_tmp_prod(now, num);
      prod *= -1;
      prod += mod;
      c = prod * inverse(i);
      c %= mod;
      for (size_t j = 0; j < num; ++j) {
        log[j] += (c * safe_get(now, j) % mod);
        log[j] %= mod;
        make_pos(log[j]);
      }
    }
    data = log;
    return *this;
  }

  gen_func &to_sqrt_(size_t num) {
    std::vector<int64_t> sqrt(num, 0);
    sqrt[0] = 1;
    int64_t c = 0;
    std::vector<int64_t> now(1, 1);
    for (size_t i = 1; i < num; ++i) {
      now = get_tmp_prod(now, num);
      int64_t a = 1;
      int64_t b = 1;
      for (int64_t l = 0; l < i; ++l) {
        a *= (1 - 2 * l + mod) % mod;
        b *= ((l + 1) * 2) % mod;
        a %= mod;
        b %= mod;
      }
      c = a * pow_mod(b, mod - 2);
      c %= mod;
      make_pos(c);
      for (size_t j = 0; j < num; ++j) {
        sqrt[j] += (c * safe_get(now, j)) % mod;
        sqrt[j] %= mod;
        make_pos(sqrt[j]);
      }
    }
    data = sqrt;
    return *this;
  }
};

std::ostream &operator<<(std::ostream &s, gen_func const &in) {
  for (auto i : in.data) {
    s << i << " ";
  }
  return s;
}

gen_func operator+(gen_func f, gen_func const &s) {
  return f += s;
}

gen_func operator*(gen_func f, gen_func const &s) {
  return f *= s;
}

gen_func divide(gen_func f, gen_func const &s, size_t num) {
  return f.div_(s, num);
}

gen_func to_sqrt(gen_func f, size_t num) {
  return f.to_sqrt_(num);
}

gen_func to_exp(gen_func f, size_t num) {
  return f.to_exp_(num);
}

gen_func to_log(gen_func f, size_t num) {
  return f.to_log_(num);
}

int main() {
  size_t k, m;
  std::cin >> k >> m;
  std::vector<bool> has(m + 1, false);
  size_t c = 0;
  for (size_t i = 0; i < k; ++i) {
    std::cin >> c;
    has[c] = true;
  }
  std::vector<uint64_t> answer(m + 1, 0), buffer(m + 1, 0);
  answer[0] = buffer[0] = 1;
  for (size_t i = 1; i < m + 1; ++i) {
    for (size_t j = 0; j < i; ++j) {
      if (has[j + 1]) {
        answer[i] += buffer[i - j - 1] % gen_func::mod;
        answer[i] %= gen_func::mod;
      }
    }
    for (size_t j = 0; j < i + 1; ++j) {
      buffer[i] += answer[j] * answer[i - j];
      buffer[i] %= gen_func::mod;
    }
  }
  for (size_t i = 1; i < m + 1; ++i) {
    std::cout << answer[i] << ' ';
  }
}