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


int main() {
  auto ans = pq_seq();
  size_t k;
  uint64_t r;
  std::cin >> r >> k;
  ++k;
  int64_t c;
  uint64_t r_pow = 1;
  uint64_t factor = 1;
  int64_t l_g = 1;
  int64_t r_g = k;
  for (size_t i = 1; i < k; ++i) {
    factor *= i;
  }
  for (size_t i = 0; i < k; ++i) {
    std::cin >> c;
    auto func = gen_func(l_g, r_g);
    auto tmp = pq_seq{func.get_data()};
    ans += ((tmp * c) * pq{1, r_pow, false}) * pq{1, factor, false};
    r_pow *= r;
    --l_g;
    --r_g;
  }
  std::cout << ans;
  size_t size = ans.size();
  while(size < k) {
    std::cout << " " << "0/1";
    ++size;
  }
  std::cout << std::endl;
}