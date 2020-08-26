#include <iostream>
#include <vector>
#include <algorithm>
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
 public:

  static void make_pos(int64_t &i) {
    if (i < 0) {
      i += mod;
    }
  }

  static const int64_t mod = 104857601;

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

  static void get_tmp_prod(std::vector<int64_t> &f, std::vector<int64_t> &s, size_t k) {
    for (size_t i = k; i < (k << 1u); ++i) {
      f[i] = 0;
      for (size_t j = 1; j < k + 1; ++j) {
        f[i] -= s[j] * f[i - j];
        f[i] %= gen_func::mod;
        make_pos(f[i]);
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &, const gen_func &);
};

std::ostream &operator<<(std::ostream &s, gen_func const &in) {
  for (auto i : in.data) {
    s << i << " ";
  }
  return s;
}

int main() {
  std::cout.tie(nullptr);
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  size_t k = 0;
  uint64_t need_number = 0;
  std::cin >> k >> need_number;
  std::vector<int64_t> a;
  std::vector<int64_t> q;
  std::vector<int64_t> q_minus, q_new;
  q_minus.resize(k + 1, 0);
  q_new.resize(k + 1, 0);
  q.reserve(k + 1);
  a.resize(k << 1u, 0);
  q.push_back(1);
  --need_number;
  int64_t w;
  for (size_t i = 0; i < k; ++i) {
    std::cin >> a[i];
  }
  for (size_t i = 0; i < k; ++i) {
    std::cin >> w;
    q.push_back((gen_func::mod - w) % gen_func::mod);
  }
  while (need_number >= k) {
    gen_func::get_tmp_prod(a, q, k);
    std::copy(q.begin(), q.end(), q_minus.begin());
    std::vector<std::future<void>> result;
    for (size_t i = 1; i < k + 1; i += 2) {
      result.push_back(std::move(
          std::async(std::launch::deferred, [i, &q, &q_minus] () {q_minus[i] = (gen_func::mod - q[i]) % gen_func::mod;}
      )));
    }
    for (auto & i : result) {
      i.get();
    }
    for (size_t i = 0; i < k + 1; ++i) {
      q_new[i] = 0;
      for (size_t j = 0; j < (i << 1u) + 1; ++j) {
        if (((i << 1u) < (k + j + 1)) && (j < k + 1)) {
          q_new[i] += q[j] * q_minus[(i << 1u) - j];
          q_new[i] %= gen_func::mod;
          gen_func::make_pos(q_new[i]);
        }
      }
    }
    std::copy(q_new.begin(), q_new.end(), q.begin());
    std::vector<int64_t> new_a{a};
    size_t last_idx = 0;
    for (size_t i = need_number % 2u; i < (k << 1u); i += 2) {
      new_a[last_idx] = a[i];
      ++last_idx;
    }
    std::swap(a, new_a);
    need_number >>= 1u;
  }
  std::cout << a[need_number] << std::endl;
}
/*
4 10000
1 8 10 12
4 1 10 90
*/