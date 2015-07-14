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

  auto empty() const -> bool {
    return data == nullptr;
  }

  auto get() const -> string {
    switch(type) {
    case Type::Bool: return {*(bool*)data};
    case Type::Signed: return {*(signed*)data};
    case Type::Unsigned: return {*(unsigned*)data};
    case Type::Double: return {*(double*)data};
    case Type::String: return {*(string*)data};
    }
    return "";
  }

  auto set(const string& value) -> void {
    switch(type) {
    case Type::Bool: *(bool*)data = (value != "false"); break;
    case Type::Signed: *(signed*)data = integer(value); break;
    case Type::Unsigned: *(unsigned*)data = decimal(value); break;
    case Type::Double: *(double*)data = real(value); break;
    case Type::String: *(string*)data = value; break;
    }
  }

  auto assign() { type = Type::Null; data = nullptr; }
  auto assign(bool& bind) { type = Type::Bool; data = (void*)&bind; }
  auto assign(signed& bind) { type = Type::Signed; data = (void*)&bind; }
  auto assign(unsigned& bind) { type = Type::Unsigned; data = (void*)&bind; }
  auto assign(double& bind) { type = Type::Double; data = (void*)&bind; }
  auto assign(string& bind) { type = Type::String; data = (void*)&bind; }
  auto assign(const Node& node) { operator=(node); }

  template<typename T> auto append(T& data, const string& name, const string& desc = "") -> void {
    Node node;
    node.assign(data);
    node.name = name;
    node.desc = desc;
    children.append(node);
  }

  auto load(Markup::Node path) -> void {
    for(auto& child : children) {
      if(auto leaf = path[child.name]) {
        if(!child.empty()) child.set(leaf.text());
        child.load(leaf);
      }
    }
  }

  auto save(file& fp, unsigned depth = 0) -> void {
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
  auto load(const string& filename) -> bool {
    if(!file::exists(filename)) return false;
    auto document = BML::unserialize(string::read(filename));
    Node::load(document);
    return true;
  }

  auto save(const string& filename) -> bool {
    file fp(filename, file::mode::write);
    if(!fp.open()) return false;
    Node::save(fp);
    return true;
  }
};

}
}

#endif
