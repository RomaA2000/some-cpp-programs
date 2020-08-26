#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

using T = double;

class complex {
 public:
  T real;
  T im;

  complex() : real(), im() {}
  complex(T real, T im) : real(real), im(im) {}
  explicit complex(T real) : real(real), im() {}
  complex &operator+=(complex const &other) {
    im += other.im;
    real += other.real;
    return *this;
  }

  complex &operator*=(complex const &other) {
    complex ans;
    ans.real = real * other.real - im * other.im;
    ans.im = real * other.im + im * other.real;
    std::swap(ans, *this);
    return *this;
  }

  void swap(complex &a, complex &b) {
    std::swap(a.real, b.real);
    std::swap(a.im, b.im);
  }

  friend complex operator*(complex, complex const &);

  static std::vector<complex> complex_n_roots(size_t n) {
    std::vector<complex> ans;
    for (size_t i = 0; i < n; ++i) {
      auto val = (T) (M_PI * 2.0 * (T) i) / ((T) n);
      ans.emplace_back(cos(val), sin(val));
    }
    return ans;
  }

  [[nodiscard]] std::string to_string() const {
    return std::to_string(real) + "  " + std::to_string(im);
  }

  void div(size_t n) {
    real /= (T) n;
    im /= (T) n;
  }
};

using number = std::vector<complex>;

complex operator*(complex first, complex const &second) {
  first *= second;
  return first;
}

complex operator+(complex first, complex const &second) {
  first += second;
  return first;
}

class Prod {
 public:
  static number calc_impl(number const &other, size_t n) {
    if (n == 1) {
      return {other[0]};
    }
    number a0;
    auto half = n >> 1u;
    a0.reserve(half);
    for (size_t i = 0; i < n; i += 2) {
      a0.push_back(other[i]);
    }
    number a1;
    a1.reserve(half);
    for (size_t i = 1; i < n; i += 2) {
      a1.push_back(other[i]);
    }
    auto y0 = calc_impl(a0, half);
    auto y1 = calc_impl(a1, half);
    number y;
    y.reserve(n);
    T val = (T) (M_PI * 2.0) / ((T) n);
    complex base = complex(1);
    complex w = complex(cos(val), sin(val));
    for (auto i : y0) {
      std::cout << i.to_string() << " . ";
    }
    std::cout << std::endl;
    for (auto i : y1) {
      std::cout << i.to_string() << " . ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < n; ++i) {
      y.push_back(y0[i % half] + base * y1[i % half]);
      base *= w;
    }
    return y;
  }

  static number back_impl(number const &other, size_t n) {
    auto b = calc_impl(other, n);
    number a;
    a.reserve(n);
    for (auto &i : b) {
      i.div(n);
    }
    a.push_back(b[0]);
    for (size_t i = 1; i < n; ++i) {
      a.push_back(b[n - i]);
    }
    return a;
  }

  static number calc(number const &other) {
    return calc_impl(other, other.size());
  }

  static number back(number const &other) {
    return back_impl(other, other.size());
  }
};

int main() {
  size_t result_size = 4;
  number nums_1 = {complex(3), complex(1), complex(0), complex(0)};
  auto ans = Prod::calc(nums_1);
  for (auto i : ans) {
    std::cout << i.to_string() << " . ";
  }
  std::cout << std::endl;
  return 0;
}
