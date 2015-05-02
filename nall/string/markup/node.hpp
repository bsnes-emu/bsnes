#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {
namespace Markup {

struct Node;
struct ManagedNode;
using SharedNode = shared_pointer<ManagedNode>;

struct ManagedNode {
  ManagedNode() = default;
  ManagedNode(const string& name) : _name(name) {}
  ManagedNode(const string& name, const string& value) : _name(name), _value(value) {}

  auto clone() const -> SharedNode {
    SharedNode clone(new ManagedNode(_name, _value));
    for(auto& child : _children) clone->_children.append(child->clone());
    return clone;
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

  explicit operator bool() const { return shared->_name || shared->_children; }
  auto name() const -> string { return shared->_name; }
  auto value() const -> string { return shared->_value; }

  auto text() const -> string { return value().strip(); }
  auto boolean() const -> bool { return text() == "true"; }
  auto integer() const -> intmax_t { return text().numeral(); }
  auto decimal() const -> uintmax_t { return text().numeral(); }

  auto setName(const string& name = "") -> void { shared->_name = name; }
  auto setValue(const string& value = "") -> void { shared->_value = value; }

  auto reset() -> void { shared->_children.reset(); }
  auto size() const -> unsigned { return shared->_children.size(); }

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

  auto at(unsigned position) const -> Node {
    if(position >= size()) return {};
    return shared->_children[position];
  }

  auto swap(unsigned x, unsigned y) -> bool {
    if(x >= size() || y >= size()) return false;
    return std::swap(shared->_children[x], shared->_children[y]), true;
  }

  auto insert(unsigned position, const Node& node) -> bool {
    if(position > size()) return false;  //used > instead of >= to allow indexed-equivalent of append()
    return shared->_children.insert(position, node.shared), true;
  }

  auto remove(unsigned position) -> bool {
    if(position >= size()) return false;
    return shared->_children.remove(position), true;
  }

  auto operator()(const string& path) -> Node { return shared->_create(path); }
  auto operator[](const string& path) const -> Node { return shared->_lookup(path); }
  auto find(const string& query) const -> vector<Node> { return shared->_find(query); }

  struct iterator {
    auto operator*() -> Node { return {source.shared->_children[position]}; }
    auto operator!=(const iterator& source) const -> bool { return position != source.position; }
    auto operator++() -> iterator& { return position++, *this; }
    iterator(const Node& source, unsigned position) : source(source), position(position) {}

  private:
    const Node& source;
    unsigned position;
  };

  auto begin() const -> iterator { return iterator(*this, 0); }
  auto end() const -> iterator { return iterator(*this, size()); }

protected:
  SharedNode shared;
};

}
}

namespace nall {

inline range_t range(const Markup::Node& node) {
  return range_t{0, (signed)node.size(), 1};
}

}

#endif
