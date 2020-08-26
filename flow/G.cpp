//
// Created by Роман Агеев on 19.12.2019.
//

#include <vector>
#include <iostream>
#include <functional>

template<typename T>
using matrix = std::vector<std::vector<T>>;
using umatrix64_t = matrix<uint64_t>;

class Vengerian {
  umatrix64_t array;

 public:
  Vengerian(umatrix64_t const &in) {
    array.assign(in.size() + 1, std::vector<uint64_t>(in.size() + 1, 0u));
    for (size_t i = 0; i < in.size(); ++i) {
      for (size_t j = 0; j < in[i].size(); ++j) {
        array[i + 1][j + 1] = in[i][j];
      }
    }
  }

  void iterate_over(std::function<void(size_t)> const &f, size_t l, size_t r) {
    for (; l < r; ++l) {
      f(l);
    }
  }

  std::pair<uint64_t, std::vector<size_t>> solve() {
    size_t n = array.size();
    std::vector<int64_t> rows(n), columns(n);
    std::vector<size_t> now_p(n), way(n);
    std::vector<int64_t> min_columns;
    std::vector<bool> was;
    for (size_t i = 1; i < n; ++i) {
      min_columns.assign(n, std::numeric_limits<int64_t>::max() / 1000000);
      was.assign(n, false);
      size_t first_j = 0;
      now_p[0] = i;
      do {
        int64_t delta = std::numeric_limits<int64_t>::max() / 1000000;
        size_t second_j = 0;
        was[first_j] = true;
        size_t first_i = now_p[first_j];
        iterate_over(
            [&](size_t j) {
              if (!was[j]) {
                int64_t cur = array[first_i][j] - columns[j] - rows[first_i];
                if (cur < min_columns[j]) {
                  way[j] = first_j;
                  min_columns[j] = cur;
                }
                if (min_columns[j] < delta) {
                  second_j = j;
                  delta = min_columns[j];
                }
              }
            }, 1, n);
        iterate_over([&] (size_t j) {
          if (!was[j]) {
            min_columns[j] -= delta;
          } else {
            columns[j] -= delta;
            rows[now_p[j]] += delta;
          }
        }, 0, n);
        first_j = second_j;
      } while (now_p[first_j] != 0);
      do {
        now_p[first_j] = now_p[way[first_j]];
        first_j = way[first_j];
      } while (first_j != 0);
    }
    std::pair<uint64_t, std::vector<size_t>> ans;
    ans.first = static_cast<uint64_t>(-columns[0]);
    ans.second.resize(n - 1);
    for (size_t j = 1; j < n + 1; ++j) {
      ans.second[now_p[j] - 1] = j - 1;
    }
    return ans;
  }
};

int main() {
  size_t n = 0;
  std::cin >> n;
  if (n != 0) {
    umatrix64_t matrix{};
    matrix.resize(n, std::vector<uint64_t>(n));
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        std::cin >> matrix[i][j];
      }
    }
    Vengerian vengerian(matrix);
    auto[ans_num, array] = vengerian.solve();
    std::cout << ans_num << std::endl;
    for (size_t i = 0; i < array.size(); ++i) {
      std::cout << i + 1 << ' ' << array[i] + 1 << '\n';
    }
  }
  return 0;
}
/*
4
2 1 3 2
3 2 1 3
2 1 3 1
1 3 2 2
*/
