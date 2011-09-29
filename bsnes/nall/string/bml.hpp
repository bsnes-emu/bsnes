#ifdef NALL_STRING_INTERNAL_HPP

//BML parser
//version 0.02

namespace nall {
namespace BML {

struct Node : linear_vector<Node> {
  Node *parent;
  const char *cname;
  const char *cvalue;

  inline string name() { return cname; }
  inline string value() { return cvalue; }

private:
  inline bool valid(char p) const {
    if(p >= 'A' && p <= 'Z') return true;
    if(p >= 'a' && p <= 'z') return true;
    if(p >= '0' && p <= '9') return true;
    if(p == '+' || p == '-') return true;
    if(p == '.' || p == '_') return true;
    if(p == ':') return true;
    return false;
  }

  inline bool space(char p) const {
    return p == ' ' || p == '\t';
  }

  inline unsigned parseDepth(char *p) {
    unsigned depth = 0;
    while(space(*p)) depth++, p++;
    return depth;
  }

  inline void parseNode(char *&p) {
    if(valid(*p)) {
      cname = p;
      while(valid(*p)) p++;
      if(*p != '=') return;
      *p++ = 0;
    }
    if(*p == '\n') throw "Missing node value";

    if(valid(*p)) {
      cvalue = p;
      while(valid(*p)) p++;
    } else {
      char terminal = *p++;
      cvalue = p;
      while(*p && *p != terminal) p++;
      if(*p == 0) throw "Unclosed terminal";
      *p++ = 0;
    }
  }

  inline void parseLine(char *&p) {
    unsigned depth = parseDepth(p);
    while(space(*p)) p++;
    parseNode(p);

    while(space(*p)) {
      *p++ = 0;
      Node node(this);
      node.parseNode(p);
      append(node);
    }

    if(*p == '\n') *p++ = 0;

    while(parseDepth(p) > depth) {
      Node node(this);
      node.parseLine(p);
      append(node);
    }
  }

  inline void parse(char *&p, unsigned parentDepth = 0) {
    while(*p) {
      while(*p == '\n') *p++ = 0;
      Node node(this);
      node.parseLine(p);
      append(node);
    }
  }

public:
  inline Node& operator[](const string &name) {
    for(auto &node : *this) {
      if(!strcmp(node.cname, name)) return node;
    }
    static Node node;
    return node;
  }

  inline bool exists() {
    return parent != nullptr;
  }

  inline string content(const string &separator = "\n") const {
    string result;
    for(auto &node : *this) result.append(node.cvalue, separator);
    result.rtrim<1>(separator);
    return result;
  }

  inline Node(const string &document) : parent(nullptr), cname(nullptr), cvalue(nullptr) {
    char *p = strdup(document);
    cvalue = p;
    try {
      parse(p);
    } catch(const char *error) {
      reset();
    }
  }

  inline Node(Node *parent) : parent(parent), cname(""), cvalue("") {}
  inline Node() : parent(nullptr), cname(""), cvalue("") {}
  inline ~Node() { if(cname == nullptr && cvalue) free((void*)cvalue); }
};

}
}

#endif
