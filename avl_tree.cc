#include <cstddef>
#include <iostream>
#include <queue>
#include <string>


template <typename K, typename T>
struct avl_node
{
  K key;
  T value;
  int height = 1;

  avl_node *right = nullptr;
  avl_node *left = nullptr;
};

template <typename K, typename T>
class avl_tree
{
public:
  void insert(const K &key, const T &val)
  {
    _root = _insert_impl(_root, key, val, nullptr);
  }

  int height() { return _height_of(_root); }

  T &operator[](const K &key)
  {
    avl_node<K, T> *inserted_node;
    // XXX: this `T{}` looks a little strange
    _root = _insert_impl(_root, key, T{}, &inserted_node);
    return inserted_node->value;
  }

  bool contains(const K &key)
  { return _find(_root, key) != nullptr; }

  // AI generated
  void print_bfs()
  {
    if (!_root) {
      std::cout << "[empty tree]" << std::endl;
      return;
    }

    std::queue<avl_node<K, T> *> q;
    q.push(_root);
    bool firstLevel = true;

    while (!q.empty()) {
      int levelSize = q.size();

      // 输出当前层
      for (int i = 0; i < levelSize; ++i) {
        auto current = q.front();
        q.pop();

        std::cout << current->key;

        // 添加子节点到队列
        if (current->left) {
          q.push(current->left);
        }
        if (current->right) {
          q.push(current->right);
        }

        // 同层节点之间用空格分隔（最后一个节点除外）
        if (i < levelSize - 1) {
          std::cout << " ";
        }
      }

      // 如果不是最后一层，输出分隔符#
      if (!q.empty()) {
        std::cout << " # ";
      }

      firstLevel = false;
    }
    std::cout << std::endl;
  }

  ~avl_tree()
  { _clear(_root); }

private:
  avl_node<K, T> *_root = nullptr;

  avl_node<K, T> *_find(avl_node<K, T> *node, const K &key)
  {
    if (node == nullptr) {
      return node;
    }

    if (key > node->key) {
      return _find(node->right, key);
    } else if (key < node->key) {
      return _find(node->left, key);
    } else {
      return node;
    }
  }

  int _height_of(avl_node<K, T> *node)
  { return node ? node->height : 0; }

  int _balance_of(avl_node<K, T> *node)
  {
    return node ? _height_of(node->left) - _height_of(node->right) : 0;
  }

  void _set_new_height(avl_node<K, T> *node)
  {
    if (node != nullptr) {
      node->height =
          1 + std::max(_height_of(node->left), _height_of(node->right));
    }
  }

  // returns the rotated subtree's root
  avl_node<K, T> *_right_rotate(avl_node<K, T> *node)
  {
    avl_node<K, T> *l = node->left;
    avl_node<K, T> *lr = l->right;

    l->right = node;
    node->left = lr;

    _set_new_height(node);
    _set_new_height(l);

    return l;
  }

  // returns the rotated subtree's root
  avl_node<K, T> *_left_rotate(avl_node<K, T> *node)
  {
    avl_node<K, T> *r = node->right;
    avl_node<K, T> *rl = r->left;

    r->left = node;
    node->right = rl;

    _set_new_height(node);
    _set_new_height(r);

    return r;
  }

  avl_node<K, T> *_insert_impl(avl_node<K, T> *node, const K &key, const T &val,
                               avl_node<K, T> **inserted_node)
  {
    if (node == nullptr) {
      auto result = new avl_node<K, T>;
      result->key = key;
      result->value = val;
      if (inserted_node != nullptr) {
        *inserted_node = result;
      }
      return result;
    }

    if (key > node->key) {
      node->right = _insert_impl(node->right, key, val, inserted_node);
    } else if (key < node->key) {
      node->left = _insert_impl(node->left, key, val, inserted_node);
    } else {
      *inserted_node = node;
      return node;
    }

    node->height =
        1 + std::max(_height_of(node->left), _height_of(node->right));

    int bf = _balance_of(node);

    // ll situation, use right rotate
    if (bf > 1 && key < node->left->key) {
      return _right_rotate(node);
    }

    // lr situtaion,
    if (bf > 1 && key > node->left->key) {
      node->left = _left_rotate(node->left);
      return _right_rotate(node);
    }

    // rr situation, use left rotate
    if (bf < -1 && key > node->right->key) {
      return _left_rotate(node);
    }

    // rl situation
    if (bf < -1 && key < node->right->key) {
      node->right = _right_rotate(node->right);
      return _left_rotate(node);
    }

    return node;
  }

  void _clear(avl_node<K, T> *node)
  {
    if (node) {
      _clear(node->left);
      _clear(node->right);
      node->left = nullptr;
      node->right = nullptr;
      delete node;
    }
  }
};

int main()
{
  avl_tree<int, int> tree;
  tree[3] = 6;
  tree[2] = 3;
  tree[4] = 1;
  tree[5] = 1;
  tree[8] = 7;
  return 0;
}
