#ifdef NALL_STRING_INTERNAL_HPP

/* CSS Markup Language (CML) v1.0 parser
 * revision 0.01
 */

namespace nall { namespace {

struct CML {
  CML(const string& filedata, const string& pathname);
  CML(const string& filename);
  auto output() -> string;

private:
  struct State {
    string output;
  } state;

  struct Variable {
    string name;
    string value;
  };
  vector<Variable> variables;

  auto parse(const string& filedata, const string& pathname) -> bool;
};

CML::CML(const string& filedata, const string& pathname) {
  parse(filedata, pathname);
}

CML::CML(const string& filename) {
  parse(string::read(filename), filename.pathname());
}

auto CML::output() -> string {
  return state.output;
}

auto CML::parse(const string& filedata, const string& pathname) -> bool {
  auto vendorAppend = [&](const string& name, const string& value) {
    state.output.append("  -moz-", name, ": ", value, ";\n");
    state.output.append("  -webkit-", name, ": ", value, ";\n");
  };

  for(auto& block : filedata.split("\n\n")) {
    lstring lines = block.rstrip().split("\n");
    string name = lines.takeFirst();

    if(ltrim(name, "include ")) {
      string filename{pathname, name};
      parse(string::read(filename), filename.pathname());
      continue;
    }

    if(name == "variables") {
      for(auto& line : lines) {
        auto data = line.split<1>(":").strip();
        variables.append({data(0), data(1)});
      }
      continue;
    }

    state.output.append(name, " {\n");
    for(auto& line : lines) {
      auto data = line.split<1>(":").strip();
      auto name = data(0), value = data(1);
      while(auto offset = value.find("var(")) {
        bool found = false;
        if(auto length = value.findFrom(*offset, ")")) {
          string name = value.slice(*offset + 4, *length - 4);
          for(auto& variable : variables) {
            if(variable.name == name) {
              value = {value.slice(0, *offset), variable.value, value.slice(*offset + *length + 1)};
              found = true;
              break;
            }
          }
        }
        if(!found) break;
      }
      state.output.append("  ", name, ": ", value, ";\n");
      if(name == "box-sizing") vendorAppend(name, value);
    }
    state.output.append("}\n\n");
  }

  return true;
}

}}

#endif
