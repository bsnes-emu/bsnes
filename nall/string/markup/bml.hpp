#ifdef NALL_STRING_INTERNAL_HPP

//BML v1.0 parser
//revision 0.04

namespace nall {
namespace BML {

//metadata is used to store nesting level

struct ManagedNode;
using SharedNode = shared_pointer<ManagedNode>;

struct ManagedNode : Markup::ManagedNode {
protected:
  //test to verify if a valid character for a node name
  bool valid(char p) const {  //A-Z, a-z, 0-9, -.
    return p - 'A' < 26u || p - 'a' < 26u || p - '0' < 10u || p - '-' < 2u;
  }

  //determine indentation level, without incrementing pointer
  unsigned readDepth(const char* p) {
    unsigned depth = 0;
    while(p[depth] == '\t' || p[depth] == ' ') depth++;
    return depth;
  }

  //determine indentation level
  unsigned parseDepth(const char*& p) {
    unsigned depth = readDepth(p);
    p += depth;
    return depth;
  }

  //read name
  void parseName(const char*& p) {
    unsigned length = 0;
    while(valid(p[length])) length++;
    if(length == 0) throw "Invalid node name";
    _name = substr(p, 0, length);
    p += length;
  }

  void parseData(const char*& p) {
    if(*p == '=' && *(p + 1) == '\"') {
      unsigned length = 2;
      while(p[length] && p[length] != '\n' && p[length] != '\"') length++;
      if(p[length] != '\"') throw "Unescaped value";
      _value = {substr(p, 2, length - 2), "\n"};
      p += length + 1;
    } else if(*p == '=') {
      unsigned length = 1;
      while(p[length] && p[length] != '\n' && p[length] != '\"' && p[length] != ' ') length++;
      if(p[length] == '\"') throw "Illegal character in value";
      _value = {substr(p, 1, length - 1), "\n"};
      p += length;
    } else if(*p == ':') {
      unsigned length = 1;
      while(p[length] && p[length] != '\n') length++;
      _value = {substr(p, 1, length - 1), "\n"};
      p += length;
    }
  }

  //read all attributes for a node
  void parseAttributes(const char*& p) {
    while(*p && *p != '\n') {
      if(*p != ' ') throw "Invalid node name";
      while(*p == ' ') p++;  //skip excess spaces
      if(*(p + 0) == '/' && *(p + 1) == '/') break;  //skip comments

      SharedNode node(new ManagedNode);
      unsigned length = 0;
      while(valid(p[length])) length++;
      if(length == 0) throw "Invalid attribute name";
      node->_name = substr(p, 0, length);
      node->parseData(p += length);
      node->_value.rtrim("\n");
      _children.append(node);
    }
  }

  //read a node and all of its child nodes
  void parseNode(const lstring& text, unsigned& y) {
    const char* p = text[y++];
    _metadata = parseDepth(p);
    parseName(p);
    parseData(p);
    parseAttributes(p);

    while(y < text.size()) {
      unsigned depth = readDepth(text[y]);
      if(depth <= _metadata) break;

      if(text[y][depth] == ':') {
        _value.append(substr(text[y++], depth + 1), "\n");
        continue;
      }

      SharedNode node(new ManagedNode);
      node->parseNode(text, y);
      _children.append(node);
    }

    _value.rtrim("\n");
  }

  //read top-level nodes
  void parse(const string& document) {
    lstring text = string{document}.replace("\r", "").split("\n");

    //remove empty lines and comment lines
    for(unsigned y = 0; y < text.size();) {
      unsigned x = 0;
      bool empty = true;
      while(x < text[y].size()) {
        if(text[y][x] == ' ' || text[y][x] == '\t') { x++; continue; }
        empty = (text[y][x + 0] == '/' && text[y][x + 1] == '/');
        break;
      }
      if(empty) text.remove(y);
      else y++;
    }

    unsigned y = 0;
    while(y < text.size()) {
      SharedNode node(new ManagedNode);
      node->parseNode(text, y);
      if(node->_metadata > 0) throw "Root nodes cannot be indented";
      _children.append(node);
    }
  }

  friend auto unserialize(const string&) -> Markup::Node;
};

inline auto unserialize(const string& markup) -> Markup::Node {
  SharedNode node(new ManagedNode);
  try {
    node->parse(markup);
  } catch(const char* error) {
    node.reset();
  }
  return (Markup::SharedNode&)node;
}

inline auto serialize(const Markup::Node& node, unsigned depth = 0) -> string {
  if(!node.name()) {
    string result;
    for(auto leaf : node) {
      result.append(serialize(leaf, depth));
    }
    return result;
  }

  string padding;
  padding.resize(depth * 2);
  for(auto& byte : padding) byte = ' ';

  lstring lines;
  if(auto value = node.value()) lines = value.split("\n");

  string result;
  result.append(padding);
  result.append(node.name());
  if(lines.size() == 1) result.append(":", lines[0]);
  result.append("\n");
  if(lines.size() > 1) {
    padding.append("  ");
    for(auto& line : lines) {
      result.append(padding, ":", line, "\n");
    }
  }
  for(auto leaf : node) {
    result.append(serialize(leaf, depth + 1));
  }
  return result;
}

}
}

#endif
