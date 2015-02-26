#ifdef NALL_STRING_INTERNAL_HPP

/* Document Markup Language (DML) v1.0 parser
 * revision 0.01
 */

namespace nall { namespace {

struct DML {
  struct Settings {
    bool allowHTML = true;
    bool sectioned = true;
  } settings;

  DML(const string& filedata, const string& pathname);
  DML(const string& filename);
  auto output() -> string;

private:
  struct State {
    string output;
    unsigned sections = 0;
  } state;

  auto parse(const string& filedata, const string& pathname) -> bool;
  auto parseBlock(string& block, const string& pathname) -> bool;
  auto count(const string& text, char value) -> unsigned;
  auto escape(const string& text) -> string;
  auto markup(const string& text) -> string;
};

DML::DML(const string& filedata, const string& pathname) {
  parse(filedata, pathname);
}

DML::DML(const string& filename) {
  parse(string::read(filename), filename.pathname());
}

auto DML::output() -> string {
  return state.output;
}

auto DML::parse(const string& filedata, const string& pathname) -> bool {
  auto blocks = filedata.split("\n\n");
  for(auto& block : blocks) parseBlock(block, pathname);
  if(settings.sectioned && state.sections) state.output.append("</section>\n");
  return true;
}

auto DML::parseBlock(string& block, const string& pathname) -> bool {
  if(block.rstrip().empty()) return true;
  auto lines = block.split("\n");

  //include
  if(block.beginsWith("{{include}}")) {
    string filename{pathname, block.ltrim("{{include}}").strip()};
    parse(string::read(filename), filename.pathname());
  }

  //html
  else if(ltrim(block, "{{html}}") && settings.allowHTML) {
    auto data = lines.takeFirst();
    if(ltrim(data, "{{html}} ")) state.output.append(data, "\n");
    for(auto& line : lines) {
      if(ltrim(line, "  ")) state.output.append(line, "\n");
    }
  }

  //header
  else if(block.beginsWith("# ")) {
    if(settings.sectioned) {
      if(state.sections++) state.output.append("</section>");
      state.output.append("<section>");
    }
    auto content = lines.takeFirst().ltrim("# ").split<1>(" => ");
    auto data = markup(content[0]);
    auto name = escape(content(1, data.crc32()));
    state.output.append("<header id=\"", name, "\">", data);
    for(auto& line : lines) {
      if(!line.beginsWith("# ")) continue;
      state.output.append("<span>", line.ltrim("# "), "</span>");
    }
    state.output.append("</header>\n");
  }

  //subheader
  else if(auto depth = count(block, '=')) {
    auto content = lines.takeFirst().slice(depth + 1).split<1>(" => ");
    auto data = markup(content[0]);
    auto name = escape(content(1, data.crc32()));
    if(depth <= 6) {
      state.output.append("<h", depth, " id=\"", name, "\">", data);
      for(auto& line : lines) {
        if(count(line, '=') != depth) continue;
        state.output.append("<span>", line.slice(depth + 1), "</span>");
      }
      state.output.append("</h", depth, ">\n");
    }
  }

  //contents
  else if(count(block, '-')) {
    state.output.append("<nav>\n");
    unsigned level = 0;
    for(auto& line : lines) {
      if(auto depth = count(line, '-')) {
        while(level < depth) level++, state.output.append("<ul>\n");
        while(level > depth) level--, state.output.append("</ul>\n");
        auto content = line.slice(depth + 1).split<1>(" => ");
        auto data = markup(content[0]);
        auto name = escape(content(1, data.crc32()));
        state.output.append("<li><a href=\"#", name, "\">", data, "</a></li>\n");
      }
    }
    while(level--) state.output.append("</ul>\n");
    state.output.append("</nav>\n");
  }

  //list
  else if(count(block, '*')) {
    unsigned level = 0;
    for(auto& line : lines) {
      if(auto depth = count(line, '*')) {
        while(level < depth) level++, state.output.append("<ul>\n");
        while(level > depth) level--, state.output.append("</ul>\n");
        auto data = markup(line.slice(depth + 1));
        state.output.append("<li>", data, "</li>\n");
      }
    }
    while(level--) state.output.append("</ul>\n");
  }

  //quote
  else if(count(block, '>')) {
    unsigned level = 0;
    for(auto& line : lines) {
      if(auto depth = count(line, '>')) {
        while(level < depth) level++, state.output.append("<blockquote>\n");
        while(level > depth) level--, state.output.append("</blockquote>\n");
        auto data = markup(line.slice(depth + 1));
        state.output.append(data, "\n");
      }
    }
    while(level--) state.output.append("</blockquote>\n");
  }

  //code
  else if(block.beginsWith("  ")) {
    state.output.append("<pre>");
    for(auto& line : lines) {
      if(!ltrim(line, "  ")) continue;
      state.output.append(escape(line), "\n");
    }
    state.output.rtrim("\n").append("</pre>\n");
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

auto DML::count(const string& text, char value) -> unsigned {
  for(unsigned n = 0; n < text.size(); n++) {
    if(text[n] != value) {
      if(text[n] == ' ') return n;
      break;
    }
  }
  return 0;
}

auto DML::escape(const string& text) -> string {
  string output;
  for(unsigned n = 0; n < text.size();) {
    char x = text[n++];
    if(x == '&') { output.append("&amp;"); continue; }
    if(x == '<') { output.append("&lt;"); continue; }
    if(x == '>') { output.append("&gt;"); continue; }
    if(x == '"') { output.append("&quot;"); continue; }
    output.append(x);
  }
  return output;
}

auto DML::markup(const string& text) -> string {
  string output;
  char flagStrong = 0;
  char flagEmphasis = 0;
  char flagInsert = 0;
  char flagDelete = 0;
  char flagCode = 0;

  for(unsigned n = 0; n < text.size();) {
    char x = text[n], y = text[n + 1];

    if(x == '[' && y == '\\') { output.append('['); n += 2; continue; }

    if(x == '[' && y == '*' && flagStrong == 0) { flagStrong = 1; output.append("<strong>"); n += 2; continue; }
    if(x == '*' && y == ']' && flagStrong == 1) { flagStrong = 0; output.append("</strong>"); n += 2; continue; }

    if(x == '[' && y == '/' && flagEmphasis == 0) { flagEmphasis = 1; output.append("<em>"); n += 2; continue; }
    if(x == '/' && y == ']' && flagEmphasis == 1) { flagEmphasis = 0; output.append("</em>"); n += 2; continue; }

    if(x == '[' && y == '_' && flagInsert == 0) { flagInsert = 1; output.append("<ins>"); n += 2; continue; }
    if(x == '_' && y == ']' && flagInsert == 1) { flagInsert = 0; output.append("</ins>"); n += 2; continue; }

    if(x == '[' && y == '-' && flagDelete == 0) { flagDelete = 1; output.append("<del>"); n += 2; continue; }
    if(x == '-' && y == ']' && flagDelete == 1) { flagDelete = 0; output.append("</del>"); n += 2; continue; }

    if(x == '[' && y == '|' && flagCode == 0) { flagCode = 1; output.append("<code>"); n += 2; continue; }
    if(x == '|' && y == ']' && flagCode == 1) { flagCode = 0; output.append("</code>"); n += 2; continue; }

    if(x == '[' && y == '[') {
      if(auto length = text.findFrom(n + 2, "]]")) {
        lstring content = text.slice(n + 2, *length).split<1>(" => ");
        output.append("<a href=\"", escape(content[0]), "\">", escape(content(1, content[0])), "</a>");
        n += *length + 4;
        continue;
      }
    }

    if(x == '[' && y == '{') {
      if(auto length = text.findFrom(n + 2, "}]")) {
        lstring content = text.slice(n + 2, *length).split<1>(" => ");
        output.append("<img src=\"", escape(content[0]), "\" alt=\"", escape(content(1, "")), "\">");
        n += *length + 4;
        continue;
      }
    }

    if(x == '&') { output.append("&amp;"); n++; continue; }
    if(x == '<') { output.append("&lt;"); n++; continue; }
    if(x == '>') { output.append("&gt;"); n++; continue; }
    if(x == '"') { output.append("&quot;"); n++; continue; }

    output.append(x);
    n++;
  }

  if(flagStrong) output.append("</strong>");
  if(flagEmphasis) output.append("</em>");
  if(flagInsert) output.append("</ins>");
  if(flagDelete) output.append("</del>");
  if(flagCode) output.append("</code>");
  return output;
}

}}

#endif
