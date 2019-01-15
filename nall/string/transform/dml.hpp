#pragma once

/* Document Markup Language (DML) v1.0 parser
 * revision 0.04
 */

#include <nall/location.hpp>

namespace nall { namespace {

struct DML {
  auto& setAllowHTML(bool allowHTML) { settings.allowHTML = allowHTML; return *this; }
  auto& setHost(const string& hostname) { settings.host = {hostname, "/"}; return *this; }
  auto& setPath(const string& pathname) { settings.path = pathname; return *this; }
  auto& setReader(const function<string (string)>& reader) { settings.reader = reader; return *this; }
  auto& setSectioned(bool sectioned) { settings.sectioned = sectioned; return *this; }

  auto parse(const string& filedata, const string& pathname) -> string;
  auto parse(const string& filename) -> string;

private:
  struct Settings {
    bool allowHTML = true;
    string host = "localhost/";
    string path;
    function<string (string)> reader;
    bool sectioned = true;
  } settings;

  struct State {
    string output;
    uint sections = 0;
  } state;

  auto parseDocument(const string& filedata, const string& pathname, uint depth) -> bool;
  auto parseBlock(string& block, const string& pathname, uint depth) -> bool;
  auto count(const string& text, char value) -> uint;

  auto escape(const string& text) -> string;
  auto markup(const string& text) -> string;
};

auto DML::parse(const string& filedata, const string& pathname) -> string {
  settings.path = pathname;
  parseDocument(filedata, settings.path, 0);
  return state.output;
}

auto DML::parse(const string& filename) -> string {
  if(!settings.path) settings.path = Location::path(filename);
  string document = settings.reader ? settings.reader(filename) : string::read(filename);
  parseDocument(document, settings.path, 0);
  return state.output;
}

auto DML::parseDocument(const string& filedata, const string& pathname, uint depth) -> bool {
  if(depth >= 100) return false;  //attempt to prevent infinite recursion with reasonable limit

  auto blocks = filedata.split("\n\n");
  for(auto& block : blocks) parseBlock(block, pathname, depth);
  if(settings.sectioned && state.sections && depth == 0) state.output.append("</section>\n");
  return true;
}

auto DML::parseBlock(string& block, const string& pathname, uint depth) -> bool {
  if(!block.stripRight()) return true;
  auto lines = block.split("\n");

  //include
  if(block.beginsWith("<include ") && block.endsWith(">")) {
    string filename{pathname, block.trim("<include ", ">", 1L).strip()};
    string document = settings.reader ? settings.reader(filename) : string::read(filename);
    parseDocument(document, Location::path(filename), depth + 1);
  }

  //html
  else if(block.beginsWith("<html>\n") && settings.allowHTML) {
    for(auto n : range(lines.size())) {
      if(n == 0 || !lines[n].beginsWith("  ")) continue;
      state.output.append(lines[n].trimLeft("  ", 1L), "\n");
    }
  }

  //section
  else if(block.beginsWith("# ")) {
    if(settings.sectioned) {
      if(state.sections++) state.output.append("</section>");
      state.output.append("<section>");
    }
    auto content = lines.takeLeft().trimLeft("# ", 1L).split("::", 1L).strip();
    auto data = markup(content[0]);
    auto name = escape(content(1, data.hash()));
    state.output.append("<header id=\"", name, "\">", data);
    for(auto& line : lines) {
      if(!line.beginsWith("# ")) continue;
      state.output.append("<span>", line.trimLeft("# ", 1L), "</span>");
    }
    state.output.append("</header>\n");
  }

  //header
  else if(auto depth = count(block, '=')) {
    auto content = slice(lines.takeLeft(), depth + 1).split("::", 1L).strip();
    auto data = markup(content[0]);
    auto name = escape(content(1, data.hash()));
    if(depth <= 6) {
      state.output.append("<h", depth, " id=\"", name, "\">", data);
      for(auto& line : lines) {
        if(count(line, '=') != depth) continue;
        state.output.append("<span>", slice(line, depth + 1), "</span>");
      }
      state.output.append("</h", depth, ">\n");
    }
  }

  //navigation
  else if(count(block, '-')) {
    state.output.append("<nav>\n");
    uint level = 0;
    for(auto& line : lines) {
      if(auto depth = count(line, '-')) {
        while(level < depth) level++, state.output.append("<ul>\n");
        while(level > depth) level--, state.output.append("</ul>\n");
        auto content = slice(line, depth + 1).split("::", 1L).strip();
        auto data = markup(content[0]);
        auto name = escape(content(1, data.hash()));
        state.output.append("<li><a href=\"#", name, "\">", data, "</a></li>\n");
      }
    }
    while(level--) state.output.append("</ul>\n");
    state.output.append("</nav>\n");
  }

  //list
  else if(count(block, '*')) {
    uint level = 0;
    for(auto& line : lines) {
      if(auto depth = count(line, '*')) {
        while(level < depth) level++, state.output.append("<ul>\n");
        while(level > depth) level--, state.output.append("</ul>\n");
        auto data = markup(slice(line, depth + 1));
        state.output.append("<li>", data, "</li>\n");
      }
    }
    while(level--) state.output.append("</ul>\n");
  }

  //quote
  else if(count(block, '>')) {
    uint level = 0;
    for(auto& line : lines) {
      if(auto depth = count(line, '>')) {
        while(level < depth) level++, state.output.append("<blockquote>\n");
        while(level > depth) level--, state.output.append("</blockquote>\n");
        auto data = markup(slice(line, depth + 1));
        state.output.append(data, "\n");
      }
    }
    while(level--) state.output.append("</blockquote>\n");
  }

  //code
  else if(block.beginsWith("  ")) {
    state.output.append("<pre>");
    for(auto& line : lines) {
      if(!line.beginsWith("  ")) continue;
      state.output.append(escape(line.trimLeft("  ", 1L)), "\n");
    }
    state.output.trimRight("\n", 1L).append("</pre>\n");
  }

  //divider
  else if(block.equals("---")) {
    state.output.append("<hr>\n");
  }

  //paragraph
  else {
    state.output.append("<p>", markup(block), "</p>\n");
  }

  return true;
}

auto DML::count(const string& text, char value) -> uint {
  for(uint n = 0; n < text.size(); n++) {
    if(text[n] != value) {
      if(text[n] == ' ') return n;
      break;
    }
  }
  return 0;
}

auto DML::escape(const string& text) -> string {
  string output;
  for(auto c : text) {
    if(c == '&') { output.append("&amp;"); continue; }
    if(c == '<') { output.append("&lt;"); continue; }
    if(c == '>') { output.append("&gt;"); continue; }
    if(c == '"') { output.append("&quot;"); continue; }
    output.append(c);
  }
  return output;
}

auto DML::markup(const string& s) -> string {
  string t;

  boolean strong;
  boolean emphasis;
  boolean insertion;
  boolean deletion;
  boolean code;

  uint link, linkBase;
  uint embed, embedBase;

  for(uint n = 0; n < s.size();) {
    char a = s[n];
    char b = s[n + 1];

    if(!link && !embed) {
      if(a == '*' && b == '*') { t.append(strong.flip() ? "<strong>" : "</strong>"); n += 2; continue; }
      if(a == '/' && b == '/') { t.append(emphasis.flip() ? "<em>" : "</em>"); n += 2; continue; }
      if(a == '_' && b == '_') { t.append(insertion.flip() ? "<ins>" : "</ins>"); n += 2; continue; }
      if(a == '~' && b == '~') { t.append(deletion.flip() ? "<del>" : "</del>"); n += 2; continue; }
      if(a == '|' && b == '|') { t.append(code.flip() ? "<code>" : "</code>"); n += 2; continue; }
      if(a =='\\' && b =='\\') { t.append("<br>"); n += 2; continue; }
    }

    if(!embed) {
      if(link == 0 && a == '[' && b == '[') { t.append("<a href=\""); link = 1; linkBase = n += 2; continue; }
      if(link == 1 && a == ':' && b == ':') { t.append("\">"); link = 2; n += 2; continue; }
      if(link == 1 && a == ']' && b == ']') { t.append("\">", slice(s, linkBase, n - linkBase), "</a>"); n += 2; link = 0; continue; }
      if(link == 2 && a == ']' && b == ']') { t.append("</a>"); n += 2; link = 0; continue; }
      if(link == 1 && a == '@' && b == '/') { t.append(settings.host); n += 2; continue; }
    }

    if(!link) {
      if(embed == 0 && a == '{' && b == '{') { t.append("<img src=\""); embed = 1; embedBase = n += 2; continue; }
      if(embed == 1 && a == ':' && b == ':') { t.append("\" alt=\""); embed = 2; n += 2; continue; }
      if(embed != 0 && a == '}' && b == '}') { t.append("\">"); embed = 0; n += 2; continue; }
      if(embed == 1 && a == '@' && b == '/') { t.append(settings.host); n += 2; continue; }
    }

    if(a =='\\') { t.append(b); n += 2; continue; }
    if(a == '&') { t.append("&amp;"); n++; continue; }
    if(a == '<') { t.append("&lt;"); n++; continue; }
    if(a == '>') { t.append("&gt;"); n++; continue; }
    if(a == '"') { t.append("&quot;"); n++; continue; }

    t.append(a);
    n++;
  }

  if(strong) t.append("</strong>");
  if(emphasis) t.append("</em>");
  if(insertion) t.append("</ins>");
  if(deletion) t.append("</del>");
  if(code) t.append("</code>");
  if(link == 1) t.append("\">", slice(s, linkBase, s.size() - linkBase), "</a>");
  if(link == 2) t.append("</a>");
  if(embed != 0) t.append("\">");

  return t;
}

}}
