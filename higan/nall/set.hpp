#ifndef NALL_SET_HPP
#define NALL_SET_HPP

//set
//implementation: red-black tree
//search: O(log n) average; O(log n) worst
//insert: O(log n) average; O(log n) worst
//remove: O(log n) average; O(log n) worst

#include <nall/utility.hpp>
#include <nall/vector.hpp>

namespace nall {

template<typename T> struct set {
  struct node_t {
    T value;
    bool red = 1;
    node_t* link[2] = {nullptr, nullptr};
    node_t() = default;
    node_t(const T& value) : value(value) {}
  };

  node_t* root = nullptr;
  unsigned nodes = 0;

  set& operator=(const set& source) { copy(source); return *this; }
  set& operator=(set&& source) { move(std::move(source)); return *this; }
  set(const set& source) { operator=(source); }
  set(set&& source) { operator=(std::move(source)); }
  set(std::initializer_list<T> list) { for(auto& value : list) insert(value); }
  set() = default;
  ~set() { reset(); }

  unsigned size() const { return nodes; }
  bool empty() const { return nodes == 0; }

  void reset() {
    reset(root);
    nodes = 0;
  }

  optional<T&> find(const T& value) {
    if(node_t* node = find(root, value)) return node->value;
    return false;
  }

  optional<const T&> find(const T& value) const {
    if(node_t* node = find(root, value)) return node->value;
    return false;
  }

  bool insert(const T& value) {
    unsigned count = size();
    insert(root, value);
    root->red = 0;
    return size() > count;
  }

  template<typename... Args> bool insert(const T& value, Args&&... args) {
    bool result = insert(value);
    return insert(std::forward<Args>(args)...) | result;
  }

  bool remove(const T& value) {
    unsigned count = size();
    bool done = 0;
    remove(root, &value, done);
    if(root) root->red = 0;
    return size() < count;
  }

  template<typename... Args> bool remove(const T& value, Args&&... args) {
    bool result = remove(value);
    return remove(std::forward<Args>(args)...) | result;
  }

  struct base_iterator {
    bool operator!=(const base_iterator& source) const { return position != source.position; }

    base_iterator& operator++() {
      if(++position >= source.size()) { position = source.size(); return *this; }

      if(stack.last()->link[1]) {
        stack.append(stack.last()->link[1]);
        while(stack.last()->link[0]) stack.append(stack.last()->link[0]);
      } else {
        node_t* child;
        do child = stack.take();
        while(child == stack.last()->link[1]);
      }

      return *this;
    }

    base_iterator(const set& source, unsigned position) : source(source), position(position) {
      node_t* node = source.root;
      while(node) {
        stack.append(node);
        node = node->link[0];
      }
    }

  protected:
    const set& source;
    unsigned position;
    vector<node_t*> stack;
  };

  struct iterator : base_iterator {
    T& operator*() const { return base_iterator::stack.last()->value; }
    iterator(const set& source, unsigned position) : base_iterator(source, position) {}
  };

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, size()); }

  struct const_iterator : base_iterator {
    const T& operator*() const { return base_iterator::stack.last()->value; }
    const_iterator(const set& source, unsigned position) : base_iterator(source, position) {}
  };

  const const_iterator begin() const { return const_iterator(*this, 0); }
  const const_iterator end() const { return const_iterator(*this, size()); }

private:
  void reset(node_t*& node) {
    if(!node) return;
    if(node->link[0]) reset(node->link[0]);
    if(node->link[1]) reset(node->link[1]);
    delete node;
    node = nullptr;
  }

  void copy(const set& source) {
    reset();
    copy(root, source.root);
    nodes = source.nodes;
  }

  void copy(node_t*& target, const node_t* source) {
    if(!source) return;
    target = new node_t(source->value);
    target->red = source->red;
    copy(target->link[0], source->link[0]);
    copy(target->link[1], source->link[1]);
  }

  void move(set&& source) {
    root = source.root;
    nodes = source.nodes;
    source.root = nullptr;
    source.nodes = 0;
  }

  node_t* find(node_t* node, const T& value) const {
    if(node == nullptr) return nullptr;
    if(node->value == value) return node;
    return find(node->link[node->value < value], value);
  }

  bool red(node_t* node) const { return node && node->red; }
  bool black(node_t* node) const { return !red(node); }

  void rotate(node_t*& a, bool dir) {
    node_t*& b = a->link[!dir];
    node_t*& c = b->link[dir];
    a->red = 1, b->red = 0;
    std::swap(a, b);
    std::swap(b, c);
  }

  void rotateTwice(node_t*& node, bool dir) {
    rotate(node->link[!dir], !dir);
    rotate(node, dir);
  }

  void insert(node_t*& node, const T& value) {
    if(!node) { nodes++; node = new node_t(value); return; }
    if(node->value == value) { node->value = value; return; }  //prevent duplicate entries

    bool dir = node->value < value;
    insert(node->link[dir], value);
    if(black(node->link[dir])) return;

    if(red(node->link[!dir])) {
      node->red = 1;
      node->link[0]->red = 0;
      node->link[1]->red = 0;
    } else if(red(node->link[dir]->link[dir])) {
      rotate(node, !dir);
    } else if(red(node->link[dir]->link[!dir])) {
      rotateTwice(node, !dir);
    }
  }

  void balance(node_t*& node, bool dir, bool& done) {
    node_t* p = node;
    node_t* s = node->link[!dir];
    if(!s) return;

    if(red(s)) {
      rotate(node, dir);
      s = p->link[!dir];
    }

    if(black(s->link[0]) && black(s->link[1])) {
      if(red(p)) done = 1;
      p->red = 0, s->red = 1;
    } else {
      bool save = p->red;
      bool head = node == p;

      if(red(s->link[!dir])) rotate(p, dir);
      else rotateTwice(p, dir);

      p->red = save;
      p->link[0]->red = 0;
      p->link[1]->red = 0;

      if(head) node = p;
      else node->link[dir] = p;

      done = 1;
    }
  }

  void remove(node_t*& node, const T* value, bool& done) {
    if(!node) { done = 1; return; }

    if(node->value == *value) {
      if(!node->link[0] || !node->link[1]) {
        node_t* save = node->link[!node->link[0]];

        if(red(node)) done = 1;
        else if(red(save)) save->red = 0, done = 1;

        nodes--;
        delete node;
        node = save;
        return;
      } else {
        node_t* heir = node->link[0];
        while(heir->link[1]) heir = heir->link[1];
        node->value = heir->value;
        value = &heir->value;
      }
    }

    bool dir = node->value < *value;
    remove(node->link[dir], value, done);
    if(!done) balance(node, dir, done);
  }
};

}

#endif
