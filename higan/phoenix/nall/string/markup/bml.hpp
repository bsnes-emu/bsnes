#ifdef NALL_STRING_INTERNAL_HPP

//BML v1.0 parser
//revision 0.02

namespace nall {
namespace BML {

struct Node : Markup::Node {
protected:
  //test to verify if a valid character for a node name
  bool valid(char p) const {  //A-Z, a-z, 0-9, -./
    return p - 'A' < 26u || p - 'a' < 26u || p - '0' < 10u || p - '-' < 3u;
  }

  //determine indentation level, without incrementing pointer
  unsigned readDepth(const char *p) {
    unsigned depth = 0;
    while(p[depth] == '\t' || p[depth] == ' ') depth++;
    return depth;
  }

  //determine indentation level
  unsigned parseDepth(const char *&p) {
    unsigned depth = readDepth(p);
    p += depth;
    return depth;
  }

  //read name
  void parseName(const char *&p) {
    unsigned length = 0;
    while(valid(p[length])) length++;
    if(length == 0) throw "Invalid node name";
    name = substr(p, 0, length);
    p += length;
  }

  void parseData(const char *&p) {
    if(*p == '=' && *(p + 1) == '\"') {
      unsigned length = 2;
      while(p[length] && p[length] != '\n' && p[length] != '\"') length++;
      if(p[length] != '\"') throw "Unescaped value";
      data = substr(p, 2, length - 2);
      p += length + 1;
    } else if(*p == '=') {
      unsigned length = 1;
      while(p[length] && p[length] != '\n' && p[length] != '\"' && p[length] != ' ') length++;
      if(p[length] == '\"') throw "Illegal character in value";
      data = substr(p, 1, length - 1);
      p += length;
    } else if(*p == ':') {
      unsigned length = 1;
      while(p[length] && p[length] != '\n') length++;
      data = {substr(p, 1, length - 1), "\n"};
      p += length;
    }
  }

  //read all attributes for a node
  void parseAttributes(const char *&p) {
    while(*p && *p != '\n') {
      if(*p != ' ') throw "Invalid node name";
      while(*p == ' ') p++;  //skip excess spaces

      Node node;
      node.attribute = true;
      unsigned length = 0;
      while(valid(p[length])) length++;
      if(length == 0) throw "Invalid attribute name";
      node.name = substr(p, 0, length);
      node.parseData(p += length);
      children.append(node);
    }
  }

  //read a node and all of its children nodes
  void parseNode(const char *&p) {
    level = parseDepth(p);
    parseName(p);
    parseData(p);
    parseAttributes(p);
    if(*p && *p++ != '\n') throw "Missing line feed";

    while(*p) {
      if(*p == '\n') { p++; continue; }

      unsigned depth = readDepth(p);
      if(depth <= level) break;

      if(p[depth] == ':') {
        p += depth;
        unsigned length = 0;
        while(p[length] && p[length] != '\n') length++;
        data.append(substr(p, 1, length - 1), "\n");
        p += length;
        continue;
      }

      Node node;
      node.parseNode(p);
      children.append(node);
    }

    data.rtrim<1>("\n");
  }

  //read top-level nodes
  void parse(const char *p) {
    while(*p) {
      Node node;
      node.parseNode(p);
      if(node.level > 0) throw "Root nodes cannot be indented";
      children.append(node);
    }
  }

  friend class Document;
};

struct Document : Node {
  string error;

  bool load(string document) {
    name = "{root}", data = "";

    try {
      document.replace("\r", "");
      while(document.position("\n\n")) document.replace("\n\n", "\n");
      parse(document);
    } catch(const char *perror) {
      error = perror;
      children.reset();
      return false;
    }
    return true;
  }

  Document(const string &document = "") {
    load(document);
  }
};

}
}

#endif
