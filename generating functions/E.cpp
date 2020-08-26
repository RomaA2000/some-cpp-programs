//
// Created by Роман Агеев on 24.04.2020.
//
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>

class gen_func {
  std::vector<int64_t> data;

  int64_t safe_get(size_t i) const {
    return i >= size() ? 0 : data[i];
  }

 public:

  std::vector<int64_t> get_data() {
    return data;
  }

  explicit gen_func(std::vector<int64_t> data) : data(std::move(data)) {}

  explicit gen_func(int64_t v) : data({v}) {}

  gen_func(int64_t l, int64_t r) : gen_func(std::vector<int64_t>({l, 1})) {
    for (int64_t i = ++l; i < r; ++i) {
      this->operator*=(gen_func{std::vector<int64_t>({i, 1})});
    }
  }

  gen_func &operator+=(gen_func const &in) {
    size_t max_size = std::max(size(), in.size());
    data.resize(max_size, 0);
    size_t min_size = std::min(size(), in.size());
    for (size_t i = 0; i < min_size; ++i) {
      data[i] += in.data[i];
    }
    return *this;
  }

  gen_func &operator*=(gen_func const &in) {
    std::vector<int64_t> ans(size() + in.size());
    for (size_t i = 0; i < size(); ++i) {
      for (size_t j = 0; j < in.size(); ++j) {
        int64_t k = ans[i + j];
        k += (data[i] * in.data[j]);
        ans[i + j] = k;
      }
    }
    data = ans;
    shrink_to_fit();
    return *this;
  }

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

  friend std::ostream &operator<<(std::ostream &, const gen_func &);
};

gen_func make_polynom_of_k(int64_t r, size_t k) {
  auto ans = gen_func(std::vector<int64_t>({1}));
  for (size_t i = 0; i < k; ++i) {
    ans *= gen_func(std::vector<int64_t>({1, -r}));
  }
  return ans;
}

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

class pq {
  uint64_t p = 0;
  uint64_t q = 1;
  bool sign = false;
 public:
  pq() = default;
  ~pq() = default;
  pq(pq const &) = default;
  pq &operator=(pq const &) = default;
  pq(pq &&) = default;
  pq &operator=(pq &&) = default;
  explicit pq(int64_t c) : pq(c, 1) {}
  explicit pq(uint64_t c) : pq(c, 1, false) {}
  pq(int64_t p, int64_t q) : pq(std::abs(p), std::abs(q), (p < 0) ^ (q < 0)) {}
  pq(uint64_t p, uint64_t q, bool sign) : p(0), q(1), sign(sign) {
    uint64_t gcd = std::gcd(p, q);
    this->p = p / gcd;
    this->q = q / gcd;
  }

  pq &factor_div(uint64_t k) {
    for (uint64_t i = 1; i < k; ++i) {
      this->operator/=(i);
    }
    return *this;
  }

  pq &invert() {
    std::swap(p, q);
    return *this;
  }

  pq &operator*=(uint64_t v) {
    uint64_t f_gcd = std::gcd(q, v);
    q /= f_gcd;
    p *= (v / f_gcd);
    return *this;
  }

  pq &operator*=(int64_t v) {
    this->operator*=((uint64_t) std::abs(v));
    sign ^= (v < 0);
    return *this;
  }

  pq &operator/=(int64_t v) {
    this->operator/=((uint64_t) std::abs(v));
    sign ^= (v < 0);
    return *this;
  }

  pq &operator/=(uint64_t v) {
    uint64_t f_gcd = std::gcd(p, v);
    p /= f_gcd;
    q *= (v / f_gcd);
    return *this;
  }

  pq &operator+=(pq const & v) {
    uint64_t denominator = std::lcm(q, v.q);
    int64_t nominator1 = p * (denominator / q) * (sign ? -1 : 1);
    int64_t nominator2 = v.p * (denominator / v.q) * (v.sign ? -1 : 1);
    int64_t sum = nominator1 + nominator2;
    *this = pq(sum, (int64_t)denominator);
    return *this;
  }

  pq &operator*=(pq const &other) {
    uint64_t f_gcd = std::gcd(q, other.p);
    uint64_t s_gcd = std::gcd(other.q, p);
    q /= f_gcd;
    p /= s_gcd;
    p *= (other.p / f_gcd);
    q *= (other.q / s_gcd);
    sign ^= other.sign;
    return *this;
  }

  pq &operator/=(pq other) {
    this->operator*=(other.invert());
    return *this;
  }

  bool is_zero() const {
    return p == 0;
  }

  uint64_t get_num() const {
    return p;
  }

  uint64_t get_denum() const {
    return q;
  }

  bool get_sign() const {
    return sign;
  }

  friend std::ostream &operator<<(std::ostream &, const pq &);
};

pq operator*(pq f, pq const &s) {
  return f *= s;
}

pq operator/(pq f, pq const &s) {
  return f /= s;
}

pq operator+(pq f, pq const &s) {
  return f += s;
}

std::ostream &operator<<(std::ostream &s, pq const &in) {
  s << (in.sign ? "-" : "") + std::to_string(in.p) + "/" + std::to_string(in.q);
  return s;
}

class pq_seq {
  std::vector<pq> data;

 public:
  pq_seq() = default;
  ~pq_seq() = default;
  pq_seq(pq_seq const &) = default;
  pq_seq &operator=(pq_seq const &) = default;
  pq_seq(pq_seq &&) = default;
  pq_seq &operator=(pq_seq &&) = default;

  explicit pq_seq(std::vector<int64_t> const & seq) : data(std::vector<pq>()) {
    for (auto i : seq) {
      data.emplace_back(i);
    }
  }

  explicit pq_seq(std::vector<std::pair<int64_t, int64_t>> const & seq) : data(std::vector<pq>()) {
    for (auto i : seq) {
      data.emplace_back(i.first, i.second);
    }
  }

  size_t size() const {
    return data.size();
  }

  pq const & back() const {
    return data.back();
  }

  bool empty() const {
    return data.empty();
  }

  pq_seq &shrink_to_fit() {
    while (!empty() && back().is_zero()) {
      data.pop_back();
    }
    return *this;
  }

  pq_seq &operator*=(int64_t v) {
    for (auto & i: data) {
      i *= v;
    }
    return *this;
  }

  pq_seq &operator*=(pq const & v) {
    for (auto & i: data) {
      i *= v;
    }
    return *this;
  }

  pq_seq &factor_div(uint64_t v) {
    for (auto & i: data) {
      i.factor_div(v);
    }
    return *this;
  }

  pq_seq &operator+=(pq_seq const &in) {
    size_t max_size = std::max(size(), in.size());
    data.resize(max_size, pq(0, 1));
    size_t min_size = std::min(size(), in.size());
    for (size_t i = 0; i < min_size; ++i) {
      data[i] += in.data[i];
    }
    shrink_to_fit();
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &, const pq_seq &);

  std::vector<int64_t> get_num() const {
    std::vector<int64_t> ans;
    for (auto & i : data) {
      ans.push_back(((int64_t)i.get_num()) * (i.get_sign() ? -1 : 1));
    }
    return ans;
  }

  uint64_t get_lcm_q() const {
    uint64_t ans = 1;
    for (auto const & i : data) {
      ans = std::lcm(ans, i.get_denum());
    }
    return ans;
  }

  uint64_t to_N() {
    uint64_t lcm = get_lcm_q();
    for (auto & i : data) {
      i *= lcm;
    }
    return lcm;
  }
};

pq_seq operator+(pq_seq f, pq_seq const &s) {
  return f += s;
}

pq_seq operator*(pq_seq f, pq const &s) {
  return f *= s;
}

pq_seq operator*(pq_seq f, int64_t s) {
  return f *= s;
}

std::ostream &operator<<(std::ostream &s, pq_seq const &in) {
  for (auto const &i : in.data) {
    s << i << " ";
  }
  return s;
}

std::ostream &operator<<(std::ostream &s, std::vector<int64_t> const &in) {
  for (auto const &i : in) {
    s << i << " ";
  }
  return s;
}

auto get_ans(size_t k, uint64_t r) {
  std::vector<int64_t> data(1, 1);
  ++k;
  auto ans = pq_seq();
  uint64_t r_pow = 1;
  uint64_t factor = 1;
  int64_t l_g = 1;
  int64_t r_g = k;
  for (size_t i = 1; i < k; ++i) {
    factor *= i;
  }
  for (auto i : data) {
    auto func = gen_func(l_g, r_g);
    auto tmp = pq_seq{func.get_data()};
    ans += ((tmp * i) * pq{1, r_pow, false}) * pq{1, factor, false};
    r_pow *= r;
    --l_g;
    --r_g;
  }
  return ans;
}

struct pq_gen_func {
  gen_func p;
  size_t k;
  int64_t r;

  void inv() {
    p *= gen_func(-1);
  }

  pq_gen_func&operator+=(pq_gen_func n) {
    if (k < n.k) {
      p *= make_polynom_of_k(r, n.k - k);
      k = n.k;
      p += n.p;
    } else if (k > n.k) {
      p += (n.p * make_polynom_of_k(r, k - n.k));
    } else {
      p += n.p;
    }
    return *this;
  }

  pq_gen_func&operator*=(int64_t n) {
    p *= gen_func(n);
    return *this;
  }
};

pq_gen_func operator*(pq_gen_func f, int64_t s) {
  return f *= s;
}

auto gen_n_i(size_t i, int64_t r) {
  std::vector<pq_gen_func> ans;
  ans.emplace_back(pq_gen_func{gen_func(1), 1, r});
  for (size_t j = 1; j < i; ++j) {
    auto first_try = get_ans(j, r);
    auto c = first_try.to_N();
    auto cf_s = first_try.get_num();
    cf_s.pop_back();
    auto ans_tmp = (pq_gen_func{gen_func(1), j + 1, r} * c);
    for (size_t l = 0; l < ans.size(); ++l) {
      ans_tmp += (ans[l] * -1 * cf_s[l]);
    }
    ans.emplace_back(ans_tmp);
  }
  return ans;
}

int main() {
  int64_t r;
  size_t k;
  std::cin >> r >> k;
  auto ans_maker = gen_n_i(k + 1, r);
  auto zero = gen_func(0);
  auto zero_pq = pq_gen_func{zero, 1, r};
  int64_t w = 0;
  int64_t factor = 1;
  for (size_t i = 1; i < k; ++i) {
    factor *= i;
  }
  for (size_t i = 0; i < k + 1; ++i) {
    std::cin >> w;
    zero_pq += ans_maker[i] * w;
  }
  std::cout << zero_pq.p.size() - 1 << std::endl;
  std::cout << (zero_pq.p) << std::endl;
  std::cout << zero_pq.k << std::endl;
  std::cout << make_polynom_of_k(r, zero_pq.k) << std::endl;
}
