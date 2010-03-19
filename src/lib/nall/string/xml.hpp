#ifndef NALL_STRING_XML_HPP
#define NALL_STRING_XML_HPP

//XML subset parser
//version 0.05

#include <nall/vector.hpp>

namespace nall {

struct xml_attribute {
  string name;
  string content;
  virtual string parse() const;
};

struct xml_element : xml_attribute {
  string parse() const;
  linear_vector<xml_attribute> attribute;
  linear_vector<xml_element> element;

protected:
  void parse_doctype(const char *&data);
  bool parse_head(string data);
  bool parse_body(const char *&data);
  friend xml_element xml_parse(const char *data);
};

inline string xml_attribute::parse() const {
  string data;
  unsigned offset = 0;

  const char *source = content;
  while(*source) {
    if(*source == '&') {
      if(strbegin(source, "&lt;"))   { data[offset++] = '<';  source += 4; continue; }
      if(strbegin(source, "&gt;"))   { data[offset++] = '>';  source += 4; continue; }
      if(strbegin(source, "&amp;"))  { data[offset++] = '&';  source += 5; continue; }
      if(strbegin(source, "&apos;")) { data[offset++] = '\''; source += 6; continue; }
      if(strbegin(source, "&quot;")) { data[offset++] = '"';  source += 6; continue; }
    }

    //reject illegal characters
    if(*source == '&') return "";
    if(*source == '<') return "";
    if(*source == '>') return "";

    data[offset++] = *source++;
  }

  data[offset] = 0;
  return data;
}

inline string xml_element::parse() const {
  string data;
  unsigned offset = 0;

  const char *source = content;
  while(*source) {
    if(*source == '&') {
      if(strbegin(source, "&lt;"))   { data[offset++] = '<';  source += 4; continue; }
      if(strbegin(source, "&gt;"))   { data[offset++] = '>';  source += 4; continue; }
      if(strbegin(source, "&amp;"))  { data[offset++] = '&';  source += 5; continue; }
      if(strbegin(source, "&apos;")) { data[offset++] = '\''; source += 6; continue; }
      if(strbegin(source, "&quot;")) { data[offset++] = '"';  source += 6; continue; }
    }

    if(strbegin(source, "<!--")) {
      signed pos = strpos(source, "-->");
      if(pos == -1) return "";
      source += pos + 3;
      continue;
    }

    if(strbegin(source, "<![CDATA[")) {
      signed pos = strpos(source, "]]>");
      if(pos == -1) return "";
      string cdata = substr(source, 9, pos - 9);
      data << cdata;
      offset += strlen(cdata);

      source += offset + 3;
      continue;
    }

    //reject illegal characters
    if(*source == '&') return "";
    if(*source == '<') return "";
    if(*source == '>') return "";

    data[offset++] = *source++;
  }

  data[offset] = 0;
  return data;
}

inline void xml_element::parse_doctype(const char *&data) {
  name = "!DOCTYPE";
  const char *content_begin = data;

  signed counter = 0;
  while(*data) {
    char value = *data++;
    if(value == '<') counter++;
    if(value == '>') counter--;
    if(counter < 0) {
      content = substr(content_begin, 0, data - content_begin - 1);
      return;
    }
  }
  throw "...";
}

inline bool xml_element::parse_head(string data) {
  data.qreplace("\t", " ");
  data.qreplace("\r", " ");
  data.qreplace("\n", " ");
  while(qstrpos(data, "  ") >= 0) data.qreplace("  ", " ");
  data.qreplace(" =", "=");
  data.qreplace("= ", "=");
  rtrim(data);

  lstring part;
  part.qsplit(" ", data);

  name = part[0];
  if(name == "") throw "...";

  for(unsigned i = 1; i < part.size(); i++) {
    lstring side;
    side.qsplit("=", part[i]);
    if(side.size() != 2) throw "...";

    xml_attribute attr;
    attr.name = side[0];
    attr.content = side[1];
    if(strbegin(attr.content, "\"") && strend(attr.content, "\"")) trim_once(attr.content, "\"");
    else if(strbegin(attr.content, "'") && strend(attr.content, "'")) trim_once(attr.content, "'");
    else throw "...";
    attribute.add(attr);
  }
}

inline bool xml_element::parse_body(const char *&data) {
  while(true) {
    if(!*data) return false;
    if(*data++ != '<') continue;
    if(*data == '/') return false;

    if(strbegin(data, "!DOCTYPE") == true) {
      parse_doctype(data);
      return true;
    }

    if(strbegin(data, "!--")) {
      signed offset = strpos(data, "-->");
      if(offset == -1) throw "...";
      data += offset + 3;
      continue;
    }

    if(strbegin(data, "![CDATA[")) {
      signed offset = strpos(data, "]]>");
      if(offset == -1) throw "...";
      data += offset + 3;
      continue;
    }

    signed offset = strpos(data, ">");
    if(offset == -1) throw "...";

    string tag = substr(data, 0, offset);
    data += offset + 1;
    const char *content_begin = data;

    bool self_terminating = false;

    if(strend(tag, "?") == true) {
      self_terminating = true;
      rtrim_once(tag, "?");
    } else if(strend(tag, "/") == true) {
      self_terminating = true;
      rtrim_once(tag, "/");
    }

    parse_head(tag);
    if(self_terminating) return true;

    while(*data) {
      unsigned index = element.size();
      xml_element node;
      if(node.parse_body(data) == false) {
        if(*data == '/') {
          signed length = data - content_begin - 1;
          if(length > 0) content = substr(content_begin, 0, length);

          data++;
          offset = strpos(data, ">");
          if(offset == -1) throw "...";

          tag = substr(data, 0, offset);
          data += offset + 1;

          tag.replace("\t", " ");
          tag.replace("\r", " ");
          tag.replace("\n", " ");
          while(strpos(tag, "  ") >= 0) tag.replace("  ", " ");
          rtrim(tag);

          if(name != tag) throw "...";
          return true;
        }
      } else {
        element.add(node);
      }
    }
  }
}

//ensure there is only one root element
inline bool xml_validate(xml_element &document) {
  unsigned root_counter = 0;

  for(unsigned i = 0; i < document.element.size(); i++) {
    string &name = document.element[i].name;
    if(strbegin(name, "?")) continue;
    if(strbegin(name, "!")) continue;
    if(++root_counter > 1) return false;
  }

  return true;
}

inline xml_element xml_parse(const char *data) {
  xml_element self;

  try {
    while(*data) {
      xml_element node;
      if(node.parse_body(data) == false) {
        break;
      } else {
        self.element.add(node);
      }
    }

    if(xml_validate(self) == false) throw "...";
    return self;
  } catch(const char*) {
    xml_element empty;
    return empty;
  }
}

}

#endif
