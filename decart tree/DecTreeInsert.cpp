//
// Created by Роман Агеев on 2019-02-23.
//

//
// Created by Роман Агеев on 2019-02-23.
//
#include <iostream>
#include "vector"
using namespace std;
vector<int> out;
struct DecTree {
  struct Node {
    int priority;
    bool hasZero;
    bool isZero;
    int size;
    int data;
    Node *left;
    Node *right;

    Node(int data) {
      priority = rand();
      size = 1;
      hasZero = 0;
      isZero = (data == -1);
      this->left = nullptr;
      this->right = nullptr;
      this->data = data;
    }

    void update() {
      size = sizeLeft() + sizeRight() + 1;
      hasZero = isZeroLeft() || isZeroRight();
    }

    bool isZeroLeft() {
      return left == nullptr ? false : (left->hasZero || left->isZero);
    }

    bool isZeroRight() {
      return right == nullptr ? false : (right->hasZero || right->isZero);
    }

    int sizeLeft() {
      return left == nullptr ? 0 : left->size;
    }

    int sizeRight() {
      return right == nullptr ? 0 : right->size;
    }
  };

  Node *find(Node *now, int pos) {
    if (now == nullptr) {
      return nullptr;
    }
    if (now->sizeLeft() == pos) {
      return now;
    } else if (now->sizeLeft() < pos) {
      return find(now->right, pos - 1 - now->sizeLeft());
    } else {
      return find(now->left, pos);
    }
  }

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
    out.push_back(now->data);
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
  explicit DecTree(int data) {
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

  void insert(int data, int position) {
    pair<Node *, Node *> done = split(root, position);
    this->root = merge(merge(done.first, new Node(data)), (done.second));
  }

  pair<DecTree *, DecTree *> split(int x) {
    pair<Node *, Node *> ans = split(root, x);
    DecTree *newTreeLeft = new DecTree(ans.first);
    DecTree *newTreeRight = new DecTree(ans.second);
    return {newTreeLeft, newTreeRight};
  }

  DecTree *merge(DecTree *right) {
    this->root = (new DecTree(merge(this->root, right->root)))->root;
    return this;
  }

  int getSize() {
    return root == nullptr ? 0 : root->size;
  }

  void intelligentInsert(int pos, int data) {
    Node *now = find(root, pos);
    if (now == nullptr) {
      return;
    }
    pair<DecTree *, DecTree *> first = split(pos);
    if (first.second->root == nullptr) {
      return;
    }
    Node *nullNode = first.second->root;
    if ((nullNode->hasZero || nullNode->isZero)) {
      int nullPos = findPos(nullNode);
      first.second->remove(nullPos);
    }
    first.second->insert(data, 0);
    this->root = first.first->merge(first.second)->root;
  }

  int findPos(Node* nullNode) {
    int nullPos = 0;
    while (true) {
      if (nullNode->isZeroLeft()) {
        nullNode = nullNode->left;
      } else if (!nullNode->isZero) {
        nullPos += nullNode->sizeLeft() + 1;
        nullNode = nullNode->right;
      } else {
        break;
      }
    }
    nullPos += nullNode->sizeLeft();
    return nullPos;
  }

  void remove(int data) {
      pair<DecTree *, DecTree *> first = split(data);
      pair<DecTree *, DecTree *> second = first.second->split(1);
      delete second.first;
      this->root = first.first->merge(second.second)->root;
    }

  void couter() {
    couterTree(root);
  }
};

int main() {
  DecTree a;
  int n, m;
  cin >> n >> m;
  for (int i = 0; i < (m); i++) {
    a.insert(-1, i);
  }
  int b;
  for (int i = 0; i < n; i++) {
    cin >> b;
    a.intelligentInsert(b - 1, i + 1);
  }
  out.clear();
  a.couter();
  int pos = 0;
  for (int j = 0; j < (int) out.size(); j++) {
    if (out[j] != -1) {
      pos = j;
    }
  }
  cout << pos + 1 << endl;
  for (int j = 0; j <= pos; j++) {
    if (out[j] == -1) {
      cout << 0 << " ";
    } else {
      cout << out[j] << " ";
    }
  }
}
