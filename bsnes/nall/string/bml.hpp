#ifdef NALL_STRING_INTERNAL_HPP

//BML v1.0 parser
//revision 0.03

namespace nall {
namespace BML {

inline static string encode(const char *s) {
  array<char> output;
  while(*s) {
    char c = *s++;
    (c == '\n') ? output.append("{lf}", 4) :
    (c ==  '{') ? output.append("{lb}", 4) :
    (c ==  '}') ? output.append("{rb}", 4) :
    output.append(c);
  }
  output.append(0);
  return output.get();
}

struct Node {
  cstring name;
  cstring value;
  signed depth;

private:
  linear_vector<Node> children;

  inline bool valid(char p) const {
    if(p >= 'A' && p <= 'Z') return true;
    if(p >= 'a' && p <= 'z') return true;
    if(p >= '0' && p <= '9') return true;
    if(p == '-' || p == '.') return true;
    return false;
  }

  inline unsigned parseDepth(char *p) {
    unsigned depth = 0;
    while(*p == '\t') depth++, p++;
    return depth;
  }

  inline void parseName(char *&p) {
    if(valid(*p)) {
      name = p;
      while(valid(*p)) p++;
    }
  }

  inline void parseValue(char *&p) {
    *p++ = 0;  //'{'
    value = p;
    char *w = p;
    while(*p) {
      if(*p == '\n') throw "Unclosed node value";
      if(*p ==  '}') break;
      if(*p ==  '{') {
             if(*(p + 1) == 'l' && *(p + 2) == 'b' && *(p + 3) == '}') *w++ =  '{', p += 4;
        else if(*(p + 1) == 'r' && *(p + 2) == 'b' && *(p + 3) == '}') *w++ =  '}', p += 4;
        else if(*(p + 1) == 'l' && *(p + 2) == 'f' && *(p + 3) == '}') *w++ = '\n', p += 4;
        else throw "Unrecognized escape sequence";
        continue;
      }
      *w++ = *p++;
    }

    if(*p != '}') throw "Unclosed node value";
    *w = *p++ = 0;  //'}'
  }

  inline void parseLine(char *&p) {
    depth = parseDepth(p);
    while(*p == '\t') p++;

    parseName(p);
    bool multiLine = false;
    if(*p == ':') {
      multiLine = true;
      *p++ = 0;
    } else if(*p == '{') {
      parseValue(p);
      if(*p != ' ' && *p != '\n') throw "Invalid character after first node value";
    }

    while(*p == ' ') {
      *p++ = 0;
      Node node;
      node.parseName(p);
      if(*p == '{') node.parseValue(p);
      if(*p != ' ' && *p != '\n') throw "Invalid character after node";
      children.append(node);
    }

    if(*p == 0) throw "Missing line feed";
    while(*p == '\n') *p++ = 0;

    if(multiLine == false) {
      while(parseDepth(p) > depth) {
        Node node;
        node.parseLine(p);
        children.append(node);
      }
    } else {
      value = p;
      char *w = p;
      while(parseDepth(p) > depth) {
        p += depth + 1;
        while(*p && *p != '\n') *w++ = *p++;
        if(*p != '\n') throw "Multi-line value missing line feed";
        *w++ = *p++;
      }
      *(w - 1) = 0;
      if(value == p) value = p - 1;  //no text data copied, point it back at null data
      *(p - 1) = 0;
    }
  }

  inline void parse(char *&p) {
    while(*p) {
      while(*p == '\n') *p++ = 0;
      Node node;
      node.parseLine(p);
      children.append(node);
    }
  }

public:
  inline Node& operator[](const char *name) {
    for(auto &node : children) {
      if(node.name == name) return node;
    }
    static Node node;
    node.name = nullptr;
    return node;
  }

  inline bool exists() const {
    return name;
  }

  inline bool load(const char *document) {
    if(document == nullptr) return false;
    char *p = strdup(document);
    name = nullptr;
    value = p;
    try {
      parse(p);
    } catch(const char *error) {
      free((void*)(const char*)value);
      children.reset();
      return false;
    }
    depth = -1;
    return true;
  }

  inline void constructor() {
    depth = 0;
    name = "";
    value = "";
  }

  unsigned size() const { return children.size(); }
  Node* begin() { return children.begin(); }
  Node* end() { return children.end(); }
  const Node* begin() const { return children.begin(); }
  const Node* end() const { return children.end(); }

  inline Node() { constructor(); }
  inline Node(const char *document) { constructor(); load(document); }
  inline ~Node() { if(depth == -1) free((void*)(const char*)value); }
};

}
}

#endif
