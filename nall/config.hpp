#ifndef NALL_CONFIG_HPP
#define NALL_CONFIG_HPP

#include <nall/platform.hpp>
#include <nall/file.hpp>
#include <nall/string.hpp>

namespace nall {
namespace Configuration {

struct Node {
  string name;
  string desc;
  enum class Type : unsigned { Null, Bool, Signed, Unsigned, Double, String } type = Type::Null;
  void* data = nullptr;
  vector<Node> children;

  bool empty() const {
    return data == nullptr;
  }

  string get() const {
    switch(type) {
    case Type::Bool: return {*(bool*)data};
    case Type::Signed: return {*(signed*)data};
    case Type::Unsigned: return {*(unsigned*)data};
    case Type::Double: return {*(double*)data};
    case Type::String: return {*(string*)data};
    }
    return "";
  }

  void set(const string& value) {
    switch(type) {
    case Type::Bool: *(bool*)data = (value == "true"); break;
    case Type::Signed: *(signed*)data = integer(value); break;
    case Type::Unsigned: *(unsigned*)data = decimal(value); break;
    case Type::Double: *(double*)data = real(value); break;
    case Type::String: *(string*)data = value; break;
    }
  }

  void assign() { type = Type::Null; data = nullptr; }
  void assign(bool& bind) { type = Type::Bool; data = (void*)&bind; }
  void assign(signed& bind) { type = Type::Signed; data = (void*)&bind; }
  void assign(unsigned& bind) { type = Type::Unsigned; data = (void*)&bind; }
  void assign(double& bind) { type = Type::Double; data = (void*)&bind; }
  void assign(string& bind) { type = Type::String; data = (void*)&bind; }
  void assign(const Node& node) { operator=(node); }

  template<typename T> void append(T& data, const string& name, const string& desc = "") {
    Node node;
    node.assign(data);
    node.name = name;
    node.desc = desc;
    children.append(node);
  }

  void load(Markup::Node path) {
    for(auto& child : children) {
      auto leaf = path[child.name];
      if(!leaf.exists()) continue;
      if(!child.empty()) child.set(leaf.data.trim<1>(" ", "\r"));
      child.load(leaf);
    }
  }

  void save(file& fp, unsigned depth = 0) {
    for(auto& child : children) {
      if(child.desc) {
        for(unsigned n = 0; n < depth; n++) fp.print("  ");
        fp.print("//", child.desc, "\n");
      }
      for(unsigned n = 0; n < depth; n++) fp.print("  ");
      fp.print(child.name);
      if(!child.empty()) fp.print(": ", child.get());
      fp.print("\n");
      child.save(fp, depth + 1);
      if(depth == 0) fp.print("\n");
    }
  }
};

struct Document : Node {
  bool load(const string& filename) {
    if(!file::exists(filename)) return false;
    auto document = Markup::Document(string::read(filename));
    Node::load(document);
    return true;
  }

  bool save(const string& filename) {
    file fp(filename, file::mode::write);
    if(!fp.open()) return false;
    Node::save(fp);
    return true;
  }
};

}
}

#endif
