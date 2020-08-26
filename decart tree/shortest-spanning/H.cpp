//
// Created by Роман Агеев on 2019-10-20.
//
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

using std::pair;
using std::cin;
using std::endl;
using std::cout;
using std::array;
using std::vector;

struct edge {
  size_t begin;
  size_t end;
  size_t cost;
};

class DSU {
 public:
  vector<size_t> classes;
  vector<size_t> len;

  DSU(size_t n) : classes(n), len(n) {
    for (size_t i = 0; i < n; ++i) {
      classes[i] = i;
    }
  }

  size_t get_class(size_t v) {
    if (classes[v] != v) {
      classes[v] = get_class(classes[v]);
      return classes[v];
    } else {
      return v;
    }
  }

  void connect(size_t a, size_t b) {
    size_t class_a = get_class(a);
    size_t class_b = get_class(b);
    if (len[class_a] <= len[class_b]) {
      classes[class_a] = class_b;
      if (len[class_a] == len[class_b]) {
        ++len[class_a];
      }
    } else {
      classes[class_b] = class_a;
    }
  }
};
int main() {
  size_t n = 0;
  cin >> n;
  size_t k = 0;
  cin >> k;
  vector<edge> line_costs(k);
  size_t a = 0, b = 0, w = 0;
  for (size_t i = 0; i < k; ++i) {
    cin >> a >> b >> w;
    line_costs[i] = {a - 1, b - 1, w};
  }
  sort(line_costs.begin(), line_costs.end(), [] (edge const &a, edge const & b) {
    return a.cost < b.cost;
  });
  DSU con(n);
  size_t ans = 0;
  for (auto e : line_costs) {
    if (con.get_class(e.begin) != con.get_class(e.end)) {
      ans += e.cost;
      con.connect(e.begin, e.end);
    }
  }
  cout << ans << endl;
}
