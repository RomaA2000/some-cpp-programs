#include <vector>
#include <algorithm>
#include <iostream>

uint64_t merge_sort_impl(std::vector<int32_t> & data, size_t l, size_t r) {
  if (r - l <= 1) {
    return 0;
  }
  size_t m = (r - l) >> 1u;
  uint64_t ans = 0;
  ans += merge_sort_impl(data, l, l + m);
  ans += merge_sort_impl(data, l + m, r);
  std::vector<int32_t> to_merge;
  to_merge.reserve(r - l);
  size_t l1 = l;
  size_t l2 = l + m;
  while ((l1 < l + m) && (l2 < r)) {
    if (data[l1] <= data[l2]) {
      to_merge.push_back(data[l1]);
      ++l1;
    } else {
      ans += l + m - l1;
      to_merge.push_back(data[l2]);
      ++l2;
    }
  }
  while (l1 < l + m) {
    to_merge.push_back(data[l1]);
    ++l1;
  }
  while (l2 < r) {
    to_merge.push_back(data[l2]);
    ++l2;
  }
  for (size_t i = l; i < r; ++i) {
    data[i] = to_merge[i - l];
  }
  return ans;
}

uint64_t merge_sort(std::vector<int32_t> & data) {
  return merge_sort_impl(data, 0, data.size());
}

struct data {
  int32_t a;
  int32_t b;
  data(int32_t a, int32_t b) : a(a), b(b) {}
};

int main() {
  size_t n;
  freopen("john.in", "r", stdin);
  freopen("john.out", "w", stdout);
  std::cin >> n;
  int32_t a, b;
  std::vector<data> vector_data;
  vector_data.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> a >> b;
    vector_data.emplace_back(a, b);
  }
  std::sort(vector_data.begin(), vector_data.end(),
            [](data const &a, data const &b) {
              if (a.a > b.a) {
                return false;
              } else if (a.a < b.a) {
                return true;
              } else {
                return a.b < b.b;
              }
            });
  std::vector<int32_t> count;
  count.reserve(n);
  for (auto i : vector_data) {
    count.push_back(i.b);
  }
  uint64_t s1 = merge_sort(count);
  std::sort(vector_data.begin(), vector_data.end(),
            [](data const &a, data const &b) {
              if (a.b > b.b) {
                return false;
              } else if (a.b < b.b) {
                return true;
              } else {
                return a.a < b.a;
              }
            });
  count.clear();
  count.reserve(n);
  for (auto i : vector_data) {
    count.push_back(i.a);
  }
  uint64_t s2 = merge_sort(count);
  std::cout << std::min(s1, s2) << std::endl;
}
