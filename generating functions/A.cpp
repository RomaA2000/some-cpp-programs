#include <iostream>
#include <vector>
#include <future>

class gen_func {
  std::vector<int64_t> data;

  int64_t safe_get(size_t i) const {
    return i >= size() ? 0 : data[i];
  }

 public:

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
        if (k < 0) {
          k += mod;
        }
        ans[i + j] = k;
      }
    }
    data = ans;
    return *this;
  }

  size_t size() const {
    return data.size();
  }

  gen_func & shrink_to_fit() {
    while(!empty() && back() == 0) {
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
      for (size_t j = 0; j < i; ++j) {
        ans[i] -= (ans[j] * s.safe_get(i - j)) % mod;
        if (ans[i] < 0) {
          ans[i] += mod;
        }
      }
      ans[i] /= s.safe_get(0);
    }
    data = ans;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &, const gen_func &);
};

std::ostream &operator<<(std::ostream &s, gen_func const &in) {
  for (auto i : in.data){
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


int main() {
  std::cout.tie(nullptr);
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  size_t n, m;
  std::cin >> n >> m;
  std::vector<int64_t> f, s;
  f.reserve(n + 1);
  s.reserve(m + 1);
  int64_t a;
  for (size_t i = 0 ; i <= n; ++i) {
    std::cin >> a;
    f.push_back(a);
  }
  for (size_t i = 0 ; i <= m; ++i) {
    std::cin >> a;
    s.push_back(a);
  }
  gen_func const f_g{std::move(f)};
  gen_func const s_g{std::move(s)};
  std::future<gen_func> f_f_sum = std::async( [&] {return (f_g + s_g).shrink_to_fit();});
  std::future<gen_func> f_f_mul = std::async( [&] {return (f_g * s_g).shrink_to_fit();});
  std::future<gen_func> f_f_div = std::async(divide, f_g, s_g, 1000u);
  auto f_sum = f_f_sum.get();
  auto sz = f_sum.size();
  std::cout <<  (sz == 0 ? 0 : sz - 1) << std::endl;
  std::cout << f_sum << std::endl;
  auto f_mul = f_f_mul.get();
  sz = f_mul.size();
  std::cout << (sz == 0 ? 0 : sz - 1) << std::endl;
  std::cout << f_mul << std::endl;
  auto f_div = f_f_div.get();
  std::cout << f_div << std::endl;
  return 0;
}
