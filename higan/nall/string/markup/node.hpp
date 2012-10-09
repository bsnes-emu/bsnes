#ifdef NALL_STRING_INTERNAL_HPP

//note: specific markups inherit from Markup::Node
//vector<Node> will slice any data; so derived nodes must not contain data nor virtual functions
//vector<Node*> would incur a large performance penalty and greatly increased complexity

namespace nall {
namespace Markup {

struct Node {
  string name;
  string data;
  bool attribute;

  bool exists() const {
    return !name.empty();
  }

  string content() const {
    return string{data}.trim(" ");
  }

  void reset() {
    children.reset();
  }

  Node& operator[](const string &name) {
    for(auto &node : *this) {
      if(node.name == name) return node;
    }
    static Node node;
    return node;
  }

  const Node& operator[](const string &name) const {
    return operator[](name);
  }

  vector<Node> operator()(const string &pattern) const {
    vector<Node> result;
    for(auto &node : *this) {
      if(node.name.wildcard(pattern)) result.append(node);
    }
    return result;
  }

  Node* begin() { return children.begin(); }
  Node* end() { return children.end(); }
  const Node* begin() const { return children.begin(); }
  const Node* end() const { return children.end(); }

  Node() : attribute(false), level(0) {}

protected:
  unsigned level;
  vector<Node> children;
};

}
}

#endif
