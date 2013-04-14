#ifdef NALL_STRING_INTERNAL_HPP

//BML v1.0 parser
//revision 0.03

namespace nall {
namespace BML {

struct Node : Markup::Node {
protected:
  //test to verify if a valid character for a node name
  bool valid(char p) const {  //A-Z, a-z, 0-9, -.
    return p - 'A' < 26u || p - 'a' < 26u || p - '0' < 10u || p - '-' < 2u;
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
      data = {substr(p, 2, length - 2), "\n"};
      p += length + 1;
    } else if(*p == '=') {
      unsigned length = 1;
      while(p[length] && p[length] != '\n' && p[length] != '\"' && p[length] != ' ') length++;
      if(p[length] == '\"') throw "Illegal character in value";
      data = {substr(p, 1, length - 1), "\n"};
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
      if(*(p + 0) == '/' && *(p + 1) == '/') break;  //skip comments

      Node node;
      node.attribute = true;
      unsigned length = 0;
      while(valid(p[length])) length++;
      if(length == 0) throw "Invalid attribute name";
      node.name = substr(p, 0, length);
      node.parseData(p += length);
      node.data.rtrim<1>("\n");
      children.append(node);
    }
  }

  //read a node and all of its child nodes
  void parseNode(const lstring &text, unsigned &y) {
    const char *p = text[y++];
    level = parseDepth(p);
    parseName(p);
    parseData(p);
    parseAttributes(p);

    while(y < text.size()) {
      unsigned depth = readDepth(text[y]);
      if(depth <= level) break;

      if(text[y][depth] == ':') {
        data.append(substr(text[y++], depth + 1), "\n");
        continue;
      }

      Node node;
      node.parseNode(text, y);
      children.append(node);
    }

    data.rtrim<1>("\n");
  }

  //read top-level nodes
  void parse(const string &document) {
    lstring text = document.split("\n");

    //remove empty lines and comment lines
    for(unsigned y = 0; y < text.size();) {
      text[y].rtrim<1>("\n");
      unsigned x = 0;
      bool empty = true;
      while(text[y][x]) {
        if(text[y][x] == ' ' || text[y][x] == '\t') { x++; continue; }
        empty = (text[y][x + 0] == '/' && text[y][x + 1] == '/');
        break;
      }
      if(empty) text.remove(y);
      else y++;
    }

    unsigned y = 0;
    while(y < text.size()) {
      Node node;
      node.parseNode(text, y);
      if(node.level > 0) throw "Root nodes cannot be indented";
      children.append(node);
    }
  }

  friend class Document;
};

struct Document : Node {
  string error;

  bool load(const string &document) {
    name = "", data = "";

    try {
      parse(document);
    } catch(const char *error) {
      this->error = error;
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
