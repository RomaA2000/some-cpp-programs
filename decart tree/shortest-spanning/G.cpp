//
// Created by Роман Агеев on 2019-10-20.
//
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <functional>

using std::pair;
using std::cin;
using std::endl;
using std::cout;
using std::array;
using std::vector;

struct edge {
  size_t begin;
  size_t end;
  double cost;
};

struct node {
  int64_t x;
  int64_t y;
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
  int64_t a = 0, b = 0;
  vector<node> nodes(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> a >> b;
    nodes[i] = {a, b};
  }
  vector<edge> line_costs;
  line_costs.reserve(n * n - n);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      edge now;
      now.begin = i;
      now.end = j;
      now.cost = sqrt((double)((nodes[i].x - nodes[j].x) * (nodes[i].x - nodes[j].x)) +
          (double)((nodes[i].y - nodes[j].y) * (nodes[i].y - nodes[j].y)));
      line_costs.push_back(now);
    }
  }
  sort(line_costs.begin(), line_costs.end(), [] (edge const &a, edge const & b) {
    return a.cost < b.cost;
  });
  DSU con(n);
  double ans = 0;
  for (auto e : line_costs) {
    if (con.get_class(e.begin) != con.get_class(e.end)) {
      ans += e.cost;
      con.connect(e.begin, e.end);
    }
  }
  cout.precision(39);
  cout << ans << endl;
}

