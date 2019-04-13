//
// Created by Роман Агеев on 2019-02-20.
//

#include <iostream>
using namespace std;

template<class Type>
class DecTree {
  struct Node {
    int priority;
    int size;
    Type data;
    Node *left;
    Node *right;

    Node(Type data) {
      priority = rand();
      size = 1;
      this->left = nullptr;
      this->right = nullptr;
      this->data = data;
    }

    void update() {
      size = sizeLeft() + sizeRight() + 1;
    }

    int sizeLeft() {
      return left == nullptr ? 0 : left->size;
    }

    int sizeRight() {
      return right == nullptr ? 0 : right->size;
    }
  };

  Node *root;

  pair<Node *, Node *> split(Node *now, int num) {
    if (now == nullptr) {
      return make_pair(nullptr, nullptr);
    }
    now->update();
    int curr = now->sizeLeft();
    if (curr < num) {
      pair<Node *, Node *> done = split(now->right, num - curr - 1);
      now->right = done.first;
      now->update();
      return {now, done.second};
    } else {
      pair<Node *, Node *> done = split(now->left, num);
      now->left = done.second;
      now->update();
      return {done.first, now};
    }
  }

  void deleteTree(Node *&x) {
    if (x == nullptr) {
      return;
    }
    if (x->left != nullptr) {
      deleteTree(x->left);
    }
    if (x->right != nullptr) {
      deleteTree(x->right);
    }
    delete x;
  }

  void couterTree(Node *now) {
    if (now == nullptr) {
      return;
    }
    if (now->left != nullptr) {
      couterTree(now->left);
    }
    cout << now->data << " ";
    if (now->right != nullptr) {
      couterTree(now->right);
    }
  }

  Node *merge(Node *left, Node *right) {
    if ((left == nullptr) || (right == nullptr)) {
      return right == nullptr ? left : right;
    }
    if ((left->priority) > (right->priority)) {
      left->right = merge(left->right, right);
      left->update();
      return left;
    } else {
      right->left = merge(left, right->left);
      right->update();
      return right;
    }
  }

 public:
  explicit DecTree(Type data) {
    root = new Node(data);
  }

  DecTree() {
    root = nullptr;
  }

  explicit DecTree(Node *in) {
    root = in;
  }

  ~DecTree() {
    deleteTree(root);
  }

  bool isEmpty() {
    return root == nullptr;
  }

  void insert(Type data) {
    pair<Node *, Node *> done = split(root, this->getSize() + 1);
    this->root = merge(merge(done.first, new Node(data)), (done.second));
  }

  pair<DecTree *, DecTree *> split(int x) {
    pair<Node *, Node *> ans = split(root, x);
    DecTree *newTreeLeft = new DecTree(ans.first);
    DecTree *newTreeRight = new DecTree(ans.second);
    return {newTreeLeft, newTreeRight};
  }

  DecTree * merge(DecTree *right) {
    this->root = (new DecTree(merge(this->root, right->root)))->root;
    return this;
  }

  int getSize() {
    return root == nullptr ? 0 : root->size;
  }

  void erase(Type data) {
    erase(root, data);
  }

  void moveToFront(int a, int b) {
    pair<DecTree *, DecTree *> first = split(b);
    pair<DecTree *, DecTree *> second = first.first->split(a);
    this->root = second.second->merge(second.first->merge(first.second))->root;
  }

  void couter() {
    couterTree(root);
  }
};

int main() {
  DecTree<int> a;
  for (int i = 10; i <= 20; i++) {
    a.insert(i);
  }
  for (int i = 0; i < 1; i++) {
    a.moveToFront(3 - 1, 5);
  }
  a.couter();
}
