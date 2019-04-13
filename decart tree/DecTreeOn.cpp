//
// Created by Роман Агеев on 2019-02-21.
//
//
// Created by Роман Агеев on 2019-02-20.
//
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Out {
  int p;
  int l;
  int r;
  int n;
};
vector<Out> ans;
struct Bit {
  int a;
  int b;
  int i;
  Bit() {
    a = 0;
    b = 0;
    i = 0;
  }
};

struct Node {
  int priority;
  int size;
  int data;
  int num;
  Node *left;
  Node *right;
  Node *parent;

  Node(int data, int p, int num) {
    priority = p;
    size = 1;
    this->num = num;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
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

  Node *getRootOfNode() {
    Node *now = this;
    while (now->parent != nullptr) {
      now = now->parent;
    }
    return now;
  }
};

struct DecTree {
  Node *root;

  pair<Node *, Node *> split(Node *now, int data) {
    if (now == nullptr) {
      return make_pair(nullptr, nullptr);
    }
    if (now->data < data) {
      pair<Node *, Node *> done = split(now->right, data);
      now->right = done.first;
      if (now->right != nullptr) {
        now->right->parent = now;
      }
      now->update();
      return make_pair(now, done.second);
    } else {
      pair<Node *, Node *> done = split(now->left, data);
      now->left = done.second;
      if (now->left != nullptr) {
        now->left->parent = now;
      }
      now->update();
      return make_pair(done.first, now);
    }
  }

  Node *find(int data) {
    Node *x = root;
    while ((x != nullptr) && (x->data != data)) {
      if (x->data < data) {
        x = x->right;
      } else {
        x = x->left;
      }
    }
    return x;
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

  Node *merge(Node *left, Node *right) {
    if ((left == nullptr) || (right == nullptr)) {
      return right == nullptr ? left : right;
    }
    if ((left->priority) > (right->priority)) {
      left->right = merge(left->right, right);
      if (left->right != nullptr) {
        left->right->parent = left;
      }
      left->update();
      return left;
    } else {
      right->left = merge(left, right->left);
      if (right->left != nullptr) {
        right->left->parent = right;
      }
      right->update();
      return right;
    }
  }

  Node *nextNode(int data) {
    Node *now = root;
    Node *answer = nullptr;
    while (now != nullptr) {
      if (now->data > data) {
        answer = now;
        now = now->left;
      } else {
        now = now->right;
      }
    }
    return answer;
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

  void insert(Node *ptr) {
    if (!search(ptr->data)) {
      pair<Node *, Node *> done = split(root, ptr->data);
      this->root = merge(merge(done.first, ptr), done.second);
    }
  }

  pair<DecTree *, DecTree *> split(int data) {
    pair<Node *, Node *> ans = split(root, data);
    if (ans.first != nullptr) {
      ans.first->parent = nullptr;
    }
    if (ans.second != nullptr) {
      ans.second->parent = nullptr;
    }
    auto *newTreeLeft = new DecTree(ans.first);
    auto *newTreeRight = new DecTree(ans.second);
    return {newTreeLeft, newTreeRight};
  }

  DecTree *merge(DecTree *right) {
    this->root = (new DecTree(merge(this->root, right->root)))->root;
    return this;
  }

  bool search(int data) {
    return (find(data) != nullptr);
  }

  void couter() {
    couterTree(root);
  }

  void couterTree(Node *now) {
    if (now == nullptr) {
      return;
    }
    Out out;

    now->parent == nullptr ? out.p = 0 : out.p = now->parent->num;
    now->right == nullptr ? out.r = 0 : out.r = now->right->num;
    now->left == nullptr ? out.l = 0 : out.l = now->left->num;
    out.n = now->num;
    ans.push_back(out);
    if (now->left != nullptr) {
      couterTree(now->left);
    }
    if (now->right != nullptr) {
      couterTree(now->right);
    }
  }
};



int main() {
  int n;
  cin >> n;
  Bit in;
  vector<Bit> arr;
  for (int i = 0; i < n; i++) {
    cin >> in.a >> in.b;
    in.i = i + 1;
    arr.push_back(in);
  }
  sort(arr.begin(), arr.end(),
       [](Bit a, Bit b) { return a.a < b.a; });
  Node *now = new Node(arr[0].a, arr[0].b, arr[0].i);
  DecTree a(now);
  for (int i = 1; i < n; i++) {
    Node *nowNode = now;
    while ((nowNode->priority > arr[i].b) && (nowNode->parent != nullptr)) {
      nowNode = nowNode->parent;
    }
    if ((nowNode->parent == nullptr) && (nowNode->priority > arr[i].b)) {
      now = new Node(arr[i].a, arr[i].b, arr[i].i);
      now->parent = nullptr;
      now->left = nowNode;
      now->right = nullptr;
      nowNode->parent = now;
      a.root = now;
    } else {
      now = new Node(arr[i].a, arr[i].b, arr[i].i);
      Node *newLeft = nowNode->right;
      nowNode->right = now;
      now->parent = nowNode;
      now->left = newLeft;
      if (newLeft != nullptr) {
        newLeft->parent = now;
      }
    }
  }
  cout << "YES" << endl;
  a.couter();
  sort(ans.begin(), ans.end(),
       [](Out a, Out b) { return a.n < b.n; });
  for (auto &an : ans) {
    cout << an.p << " " << an.l << " " << an.r << endl;
  }
}
