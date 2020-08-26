//
// Created by Роман Агеев on 05.01.2020.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <queue>
#include <unordered_set>
#include <random>

void repeat(size_t n, std::function<void(size_t)> const &f) {
  for (size_t i = 0; i < n; ++i) {
    f(i);
  }
}

class DSU {
  mutable std::vector<size_t> p;
  std::vector<uint64_t> rang;

 public:
  DSU(DSU const &) = default;
  DSU(DSU &&) = default;
  ~DSU() = default;
  DSU &operator=(DSU const &) = default;
  DSU &operator=(DSU &&) = default;
  explicit DSU(size_t n) : p(n), rang(n, 0) {
    for (size_t i = 0; i < n; ++i) {
      p[i] = i;
    }
  }

  size_t get(size_t v) const {
    if (p[v] == v) {
      return v;
    } else {
      p[v] = get(p[v]);
      return p[v];
    }
  }

  void union_vertex(size_t v, size_t u) {
    size_t pv = get(v);
    size_t pu = get(u);
    if (rang[pv] < rang[pu]) {
      p[pv] = pu;
    } else {
      p[pu] = pv;
      if (rang[pv] == rang[pu]) {
        ++rang[pv];
      }
    }
  }

  bool test(size_t v, size_t u) const {
    return get(v) == get(u);
  }
};

struct Edge {
  size_t begin;
  size_t end;
  size_t number;

  Edge(size_t begin, size_t end, size_t number) :
      begin(begin), end(end), number(number) {}

  Edge(Edge const &) = default;
  Edge(Edge &&) = default;
  ~Edge() = default;
  Edge &operator=(Edge const &) = default;
  Edge &operator=(Edge &&) = default;
};

using pair_size_t = std::pair<size_t, size_t>;
namespace std {
template<>
class hash<std::pair<size_t, size_t>> {
 public:
   size_t operator()(std::pair<size_t, size_t> const &m) const noexcept {
    return hash<size_t>()(m.first) + 239 * hash<size_t>()(m.second);
  }
};
}

using vector_trees = std::vector<std::vector<size_t>>;
using tree_mask = std::vector<std::pair<size_t, std::vector<bool>>>;
using graph_set = std::vector<std::vector<std::unordered_set<pair_size_t>>>;

class SpanningTreePacking {
  std::vector<Edge> edge_list;
  size_t n;

 public:
  SpanningTreePacking(std::vector<Edge> edge_list, size_t n) : edge_list(std::move(edge_list)), n(n) {}

  vector_trees get_ans() {
    constexpr size_t s_max = std::numeric_limits<size_t>::max();
    size_t count = 0;
    graph_set replacement_graph(1, std::vector<std::unordered_set<pair_size_t>>(n));
    tree_mask f(1, {0, std::vector<bool>(edge_list.size(), false)});
    std::vector<size_t> f_counter(1, 0);
    std::vector<size_t> indexes(edge_list.size(), s_max);
    std::vector<DSU> all_trees(1, DSU(n));
    std::function<void(size_t, size_t, std::vector<pair_size_t> & last)>
        dfs = [&replacement_graph, s_max, &dfs] (size_t i, size_t j, std::vector<pair_size_t> & last) -> void {
      for (auto [f, s] : replacement_graph[j][i]) {
        if (last[s].second == s_max) {
          last[s] = {f, i};
          dfs(s, j, last);
        }
      }
    };

    repeat(edge_list.size(), [&] (size_t i) {
      std::vector<size_t> number_in_array(edge_list.size(), s_max);
      std::vector<std::vector<pair_size_t>> last(1, std::vector<pair_size_t>(n, {s_max, s_max}));
      size_t now = edge_list[i].begin;
      size_t now_tree_num = 0;
      std::queue<size_t> queue;
      last[0][now] = {s_max, now};
      dfs(now, 0, last[0]);
      queue.push(i);
      while (now_tree_num < count && f[now_tree_num].first + 1 == n) {
        last.push_back(std::vector<pair_size_t>(n, {s_max, s_max}));
        ++now_tree_num;
        last[now_tree_num][now] = {s_max, now};
        dfs(now, now_tree_num, last[now_tree_num]);
      }

      auto iter = [&] () -> bool {
        std::vector<size_t> ready;
        while(!queue.empty()) {
          size_t now_idx = queue.front();
          size_t next_num = (indexes[now_idx] + 1) % (now_tree_num + 1);
          auto now_edge = edge_list[now_idx];
          queue.pop();
          auto last_getter = [&] (size_t i) {
            return last[next_num][i].second;
          };
          auto number_in_array_getter = [&] (size_t i) {
            return number_in_array[last[next_num][i].first];
          };

          auto updater = [&] () {
            std::vector<size_t> buff;
            bool flag1 = last_getter(now_edge.begin) != now_edge.begin;
            bool flag2 = number_in_array_getter(now_edge.begin) == s_max;
            size_t v = (flag1 && flag2) ? now_edge.begin : now_edge.end;
            while (last_getter(v) != v && number_in_array_getter(v) == s_max) {
              buff.push_back(last[next_num][v].first);
              v = last[next_num][v].second;
            }
            while (!buff.empty()) {
              size_t u = buff.back();
              buff.pop_back();
              queue.push(u);
              ready.push_back(u);
              number_in_array[u] = now_idx;
            }
          };

          auto last_case = [&] () {
            auto emplace_set = [&] () {
              replacement_graph[next_num][now_edge.begin].emplace(now_edge.number, now_edge.end);
              replacement_graph[next_num][now_edge.end].emplace(now_edge.number, now_edge.begin);
            };
            while (number_in_array[now_edge.number] != s_max) {
              emplace_set();
              replacement_graph[indexes[now_edge.number]][now_edge.begin].erase({now_edge.number, now_edge.end});
              replacement_graph[indexes[now_edge.number]][now_edge.end].erase({now_edge.number, now_edge.begin});
              size_t last_num = now_edge.number;
              now_edge = edge_list[number_in_array[now_edge.number]];
              ++f[next_num].first;
              f[next_num].second[last_num] = true;
              --f[indexes[last_num]].first;
              f[indexes[last_num]].second[last_num] = false;
              std::swap(indexes[last_num], next_num);
            }
            emplace_set();
            if(!f[next_num].second[now_edge.number]) {
              ++f[next_num].first;
              f[next_num].second[now_edge.number] = true;
            }
            std::swap(indexes[now_edge.number], next_num);
          };

          if (all_trees[next_num].test(now_edge.begin, now_edge.end)) {
            updater();
          } else {
            all_trees[next_num].union_vertex(now_edge.begin, now_edge.end);
            last_case();
            return false;
          }
        }
        ++now_tree_num;
        auto update_queue = std::async(std::launch::deferred, [&] () {
          for (auto i : ready) {
            queue.push(i);
          }
        });
        auto refill_data = std::async(std::launch::deferred, [&] () {
          if (now_tree_num > count) {
            ++count;
            f.push_back({0, std::vector<bool>(edge_list.size(), false)});
            all_trees.emplace_back(n);
            replacement_graph.emplace_back(n);
          }
          last.push_back(std::vector<pair_size_t>(n, {s_max, s_max}));
          last[now_tree_num][now] = {s_max, now};
        });
        auto dfs_getter = std::async( std::launch::deferred, [&] () {
          dfs(now, now_tree_num, last[now_tree_num]);
        });
        update_queue.get();
        refill_data.get();
        dfs_getter.get();
        return true;
      };
      while(iter());
    });
    size_t size = 0;
    while (size <= count && f[size].first + 1 == n) {
      ++size;
    }
    vector_trees answer(size, std::vector<size_t>());
    std::vector<std::future<void>> answer_pusher;
    for (size_t l = 0; l < size; ++l) {
      auto & part_f = f[l];
      auto & part_answer = answer[l];
      answer_pusher.emplace_back(std::async( [&part_f, &part_answer] () {
        for (size_t h = 0; h < part_f.second.size(); ++h) {
          if (part_f.second[h]) {
            part_answer.push_back(h);
          }
        }
      }));
    }
    for (auto & l : answer_pusher) {
      l.get();
    }
    return answer;
  }
};

int main() {

  freopen("multispan.in", "r", stdin);
  freopen("multispan.out", "w", stdout);

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t n = 0, m = 0;
  std::cin >> n >> m;
  size_t begin = 0, end = 0;
  std::vector<Edge> edges_list;
  edges_list.reserve(m);
  for (size_t i = 0; i < m; ++i) {
    std::cin >> begin >> end;
    --begin;
    --end;
    edges_list.emplace_back(begin, end, i);
  }
  SpanningTreePacking spanning_tree_packing(std::move(edges_list), n);
  auto answer = spanning_tree_packing.get_ans();
  std::reverse(answer.begin(), answer.end());
  std::cout << answer.size() << std::endl;
  for (size_t i = 0; i < answer.size(); ++i) {
    for (size_t j = 0; j < answer[i].size(); ++j) {
      std::cout << answer[i][j] + 1;
      if (j + 1 != answer[i].size()) {
        std::cout << " ";
      }
    }
    if (i + 1 != answer.size()) {
      std::cout << std::endl;
    }
  }
  return 0;
}
//int main() {
//  std::random_device rd;
//  std::mt19937 mersenne(rd());
//  uint64_t const num = 1;
//  for (size_t j = 0; j < num; ++j) {
//    size_t const n = mersenne() % 10 + 1;
//    size_t const m = mersenne() % 50 + 1;
//    std::vector<Edge> edge_list;
//    edge_list.reserve(m);
//    for (size_t i = 0; i < m; ++i) {
//      edge_list.emplace_back(mersenne() % n, mersenne() % n, i);
//    }
//    SpanningTreePacking spanning_tree_packing(std::move(edge_list), n);
//    auto ans = spanning_tree_packing.get_ans();
//  }
//}