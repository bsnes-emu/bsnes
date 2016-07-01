#pragma once

namespace nall { namespace Markup {

struct Node;
struct ManagedNode;
using SharedNode = shared_pointer<ManagedNode>;

struct ManagedNode {
  ManagedNode() = default;
  ManagedNode(const string& name) : _name(name) {}
  ManagedNode(const string& name, const string& value) : _name(name), _value(value) {}

  auto clone() const -> SharedNode {
    SharedNode clone{new ManagedNode(_name, _value)};
    for(auto& child : _children) {
      clone->_children.append(child->clone());
    }
    return clone;
  }

  auto copy(SharedNode source) -> void {
    _name = source->_name;
    _value = source->_value;
    _metadata = source->_metadata;
    _children.reset();
    for(auto child : source->_children) {
      _children.append(child->clone());
    }
  }

protected:
  string _name;
  string _value;
  uintptr_t _metadata = 0;
  vector<SharedNode> _children;

  inline auto _evaluate(string query) const -> bool;
  inline auto _find(const string& query) const -> vector<Node>;
  inline auto _lookup(const string& path) const -> Node;
  inline auto _create(const string& path) -> Node;

  friend class Node;
};

struct Node {
  Node() : shared(new ManagedNode) {}
  Node(const SharedNode& source) : shared(source ? source : new ManagedNode) {}
  Node(const string& name) : shared(new ManagedNode(name)) {}
  Node(const string& name, const string& value) : shared(new ManagedNode(name, value)) {}

  auto unique() const -> bool { return shared.unique(); }
  auto clone() const -> Node { return shared->clone(); }
  auto copy(Node source) -> void { return shared->copy(source.shared); }

  explicit operator bool() const { return shared->_name || shared->_children; }
  auto name() const -> string { return shared->_name; }
  auto value() const -> string { return shared->_value; }

  auto text() const -> string { return value().strip(); }
  auto boolean() const -> bool { return text() == "true"; }
  auto integer() const -> intmax_t { return text().integer(); }
  auto natural() const -> uintmax_t { return text().natural(); }
  auto real() const -> double { return text().real(); }

  auto setName(const string& name = "") -> Node& { shared->_name = name; return *this; }
  auto setValue(const string& value = "") -> Node& { shared->_value = value; return *this; }

  auto reset() -> void { shared->_children.reset(); }
  auto size() const -> uint { return shared->_children.size(); }

  auto prepend(const Node& node) -> void { shared->_children.prepend(node.shared); }
  auto append(const Node& node) -> void { shared->_children.append(node.shared); }
  auto remove(const Node& node) -> bool {
    for(auto n : range(size())) {
      if(node.shared == shared->_children[n]) {
        return shared->_children.remove(n), true;
      }
    }
    return false;
  }

  auto insert(uint position, const Node& node) -> bool {
    if(position > size()) return false;  //used > instead of >= to allow indexed-equivalent of append()
    return shared->_children.insert(position, node.shared), true;
  }

  auto remove(uint position) -> bool {
    if(position >= size()) return false;
    return shared->_children.remove(position), true;
  }

  auto swap(uint x, uint y) -> bool {
    if(x >= size() || y >= size()) return false;
    return std::swap(shared->_children[x], shared->_children[y]), true;
  }

  auto sort(function<bool (Node, Node)> comparator = [](auto x, auto y) {
    return string::compare(x.shared->_name, y.shared->_name) < 0;
  }) -> void {
    nall::sort(shared->_children.data(), shared->_children.size(), [&](auto x, auto y) {
      return comparator(x, y);  //this call converts SharedNode objects to Node objects
    });
  }

  auto operator[](int position) -> Node {
    if(position >= size()) return {};
    return shared->_children[position];
  }

  auto operator[](const string& path) const -> Node { return shared->_lookup(path); }
  auto operator()(const string& path) -> Node { return shared->_create(path); }
  auto find(const string& query) const -> vector<Node> { return shared->_find(query); }

  struct iterator {
    auto operator*() -> Node { return {source.shared->_children[position]}; }
    auto operator!=(const iterator& source) const -> bool { return position != source.position; }
    auto operator++() -> iterator& { return position++, *this; }
    iterator(const Node& source, uint position) : source(source), position(position) {}

  private:
    const Node& source;
    uint position;
  };

  auto begin() const -> iterator { return iterator(*this, 0); }
  auto end() const -> iterator { return iterator(*this, size()); }

protected:
  SharedNode shared;
};

}}

namespace nall {

inline range_t range(const Markup::Node& node) {
  return range_t{0, (int)node.size(), 1};
}

}
