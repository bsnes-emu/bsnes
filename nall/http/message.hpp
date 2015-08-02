#ifndef NALL_HTTP_MESSAGE_HPP
#define NALL_HTTP_MESSAGE_HPP

//httpMessage: base class for httpRequest and httpResponse
//provides shared functionality

namespace nall { namespace HTTP {

struct Variable {
  string name;
  string value;
};

struct Variables : vector<Variable> {
  auto append(const string& name, const string& value) -> void;
  auto get(const string& name) const -> string;
  auto remove(const string& name) -> void;
  auto set(const string& name, const string& value) -> void;
};

auto Variables::append(const string& name, const string& value) -> void {
  vector::append({name, value});
}

auto Variables::get(const string& name) const -> string {
  for(auto& variable : *this) {
    if(variable.name.iequals(name)) return variable.value;
  }
  return "";
}

auto Variables::remove(const string& name) -> void {
  while(true) {
    unsigned n = 0;
    bool found = false;
    for(auto& variable : *this) {
      if(!variable.name.iequals(name)) { n++; continue; }
      vector::remove(n);
      found = true;
      break;
    }
    if(found == false) break;
  }
}

auto Variables::set(const string& name, const string& value) -> void {
  for(auto& variable : *this) {
    if(!variable.name.iequals(name)) continue;
    variable.name = name;
    variable.value = value;
    return;
  }
  vector::append({name, value});
}

struct Message {
  using type = Message;

  virtual auto head(const function<bool (const uint8_t* data, unsigned size)>& callback) const -> bool = 0;
  virtual auto setHead() -> bool = 0;

  virtual auto body(const function<bool (const uint8_t* data, unsigned size)>& callback) const -> bool = 0;
  virtual auto setBody() -> bool = 0;

  virtual auto header(const string& name) const -> string { return _header.get(name); }
  virtual auto appendHeader(const string& name, const string& value = "") -> type& { return _header.append(name, value), *this; }
  virtual auto removeHeader(const string& name) -> type& { return _header.remove(name), *this; }
  virtual auto setHeader(const string& name, const string& value = "") -> type& { return _header.set(name, value), *this; }

  string _head;
  string _body;
  Variables _header;
};

}}

#endif
