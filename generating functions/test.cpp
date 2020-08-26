
#include <vector>
#include <iostream>

using namespace std;
class Node {
 public:
  int val;
  vector<Node *> children;

  Node() {}

  Node(int _val) {
    val = _val;
  }

  Node(int _val, vector<Node *> _children) {
    val = _val;
    children = _children;
  }
};

class Solution {
 public:

  static vector<int> postorder(Node *root) {
    vector<pair<Node *, bool>> stack;
    vector<int> ans;
    stack.emplace_back(root, false);
    while (!stack.empty()) {
      auto f = stack.back().first;
      auto &  b = stack.back().second;
      if (f != nullptr) {
        if (b) {
          ans.push_back(f->val);
          stack.pop_back();
        } else {
          b = true;
          std::cout << (f->val) << std::endl;
          for (auto i = f->children.crbegin(); i != f->children.crend(); ++i) {
            stack.emplace_back(*i, false);
          }
          std::cout << "all" << std::endl;
        }
      } else {
        stack.pop_back();
      }
    }
    return ans;
  }
};


int main() {
  Node test = Node{1};
  Node tes2 = Node{2};
  Node root = Node{3, {&test, &tes2, nullptr}};
  Solution::postorder(&root);
}