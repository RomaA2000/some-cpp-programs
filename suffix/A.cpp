#include <iostream>
#include <map>
#include <memory>

class Suff_trie {
 private:
  struct Node_data{
    size_t nodes;
    size_t edges;
    Node_data(size_t nodes, size_t edges) : nodes(nodes), edges(edges) {}
    Node_data() = default;
    Node_data(Node_data const &) = default;
  };

  Node_data data;

  struct Node {
    std::map<char, std::unique_ptr<Node>> child;
    size_t num;
    Node_data * data;
    Node(size_t num, Node_data *data) : num(num), data(data) {}

    Node * next(char c) {
      if (child.count(c) == 0) {
        ++data->nodes;
        child[c] = std::make_unique<Node>(data->nodes, data);
        ++data->edges;
      }
      return child[c].get();
    }
  };

  std::unique_ptr<Node> root{new Node(1, &data)};

  void print_impl(Node *now) {
    for (auto & i : now->child) {
      std::cout << now->num << " " << i.second->num << " " << i.first << std::endl;
      print_impl(i.second.get());
    }
  }

 public:
  Suff_trie(std::string const &string) : data(1, 0) {
    for (size_t i = 0; i < string.size(); ++i) {
      Node *child = root.get();
      for (size_t j = i; j < string.size(); ++j) {
        child = child->next(string[j]);
      }
    }
  }

  void solve() {
    std::cout << data.nodes << " " << data.edges << std::endl;
    print_impl(root.get());
  }
};

int main() {
  std::string in;
  std::cin >> in;
  Suff_trie suff_trie(in);
  suff_trie.solve();
}
