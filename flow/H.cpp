#include <iostream>
#include <vector>
#include <functional>
#include <deque>

struct Edge {
  size_t begin = 0;
  size_t end = 0;
  int64_t capacity = 0;
  int64_t flow = 0;
  size_t num = 0;
  int64_t cost = 0;
  Edge() = default;
  Edge(size_t begin, size_t end,
       int64_t capacity, int64_t flow, size_t num, int64_t cost)
      : begin(begin), end(end), capacity(capacity),
        flow(flow), num(num), cost(cost) {}
  Edge(Edge const &) = default;
  Edge(Edge &&) = default;
  Edge &operator=(Edge const &) = default;
};

using cost_flow = std::pair<int64_t, int64_t>;

template<typename T>
void repeat(T const &f, uint64_t n) {
  for (uint64_t i = 0; i < n; ++i) {
    f();
  }
}

class Graph {
 public:
  Graph(std::vector<std::vector<Edge>> graph)
      : graph(std::move(graph)) {
    t = graph.size() - 1;
  }

  Graph() = default;

  void add_edge(size_t begin, size_t end, int64_t cap, int64_t cost) {
    if (graph.size() < std::max(begin, end) + 1) {
      graph.resize(std::max(begin, end) + 1);
    }
    graph[begin].emplace_back(begin, end, cap, 0, graph[end].size(), cost);
    graph[end].emplace_back(end, begin, 0, 0, graph[begin].size() - 1, -cost);
    t = graph.size() - 1;
  }
  size_t t = 0;
  size_t s = 0;

  int64_t get_price() {
    int64_t ans = 0;
    if (!graph.empty()) {
      std::vector<Edge *> last(graph.size());
      std::function<cost_flow(size_t, int64_t)> f = [this, &last, &f](size_t now, int64_t flow) {
        cost_flow ans(0, flow);
        if (s != now) {
          Edge &now_edge = *last[now];
          ans = f(now_edge.begin, std::min(flow, now_edge.capacity - now_edge.flow));
          if (ans.second > 0) {
            now_edge.flow += ans.second;
            graph[now_edge.end][now_edge.num].flow -= ans.second;
            ans.first += ans.second * now_edge.cost;
          }
        }
        return ans;
      };
      while (bfs(last)) {
        auto push_value = f(t, std::numeric_limits<int64_t>::max());
        ans += push_value.first;
      }
    }
    return ans;
  }

 private:
  std::vector<std::vector<Edge>> graph;

  bool bfs(std::vector<Edge *> &last) {
    std::vector<uint64_t> dist(graph.size(), std::numeric_limits<uint64_t>::max() / 10);
    dist[s] = 0;
    repeat(
        [this, &dist, &last]() {
          for (size_t i = 0; i < graph.size(); ++i) {
            for (auto &j : graph[i]) {
              if ((j.flow != j.capacity)
              && (dist[i] != std::numeric_limits<uint64_t>::max() / 10)
                  && (dist[j.end]  > (uint64_t)dist[j.begin] + (uint64_t)j.cost)) {
                last[j.end] = &j;
                dist[j.end] = dist[i] + j.cost;
              }
            }
          }
        }, graph.size() - 1);
    return dist[t] != std::numeric_limits<uint64_t>::max() / 10;
  }
};

int main() {
  size_t n = 0, m = 0;
  std::cin >> n >> m;
  Graph graph;
  size_t begin = 0;
  size_t end = 0;
  int64_t capacity = 0;
  int64_t cost = 0;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> begin >> end >> capacity >> cost;
    --begin;
    --end;
    graph.add_edge(begin, end, capacity, cost);
  }
  std::cout << graph.get_price() << std::endl;
  return 0;
}
