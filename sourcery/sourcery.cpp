#include <nall/nall.hpp>
using namespace nall;

struct Sourcery {
  auto main(Arguments arguments) -> void;
  auto parse(Markup::Node&) -> void;

private:
  string pathname;
  file_buffer source;
  file_buffer header;
};

auto Sourcery::main(Arguments arguments) -> void {
  if(arguments.size() != 3) return print("usage: sourcery resource.bml resource.cpp resource.hpp\n");

  string markupName = arguments.take();
  string sourceName = arguments.take();
  string headerName = arguments.take();
  if(!markupName.endsWith(".bml")) return print("error: arguments in incorrect order\n");
  if(!sourceName.endsWith(".cpp")) return print("error: arguments in incorrect order\n");
  if(!headerName.endsWith(".hpp")) return print("error: arguments in incorrect order\n");

  string markup = string::read(markupName);
  if(!markup) return print("error: unable to read resource manifest\n");

  pathname = Location::path(markupName);
  if(!source.open(sourceName, file::mode::write)) return print("error: unable to write source file\n");
  if(!header.open(headerName, file::mode::write)) return print("error: unable to write header file\n");

  source.print("#include \"", headerName, "\"\n");
  source.print("\n");

  auto document = BML::unserialize(markup);
  parse(document);
}

auto Sourcery::parse(Markup::Node& root) -> void {
  for(auto node : root) {
    if(node.name() == "namespace") {
      header.print("namespace ", node["name"].text(), " {\n");
      source.print("namespace ", node["name"].text(), " {\n");
      parse(node);
      header.print("}\n");
      source.print("}\n");
    } else if(node.name() == "binary") {
      string filename{pathname, node["file"].text()};
      if(!file::exists(filename)) {
        print("warning: binary file ", node["file"].text(), " not found\n");
        continue;
      }
      auto buffer = file::read(filename);
      header.print("extern const unsigned char ", node["name"].text(), "[", buffer.size(), "];\n");
      source.print("const unsigned char ", node["name"].text(), "[", buffer.size(), "] = {\n");
      buffer.foreach([&](uint offset, int data) {
        if((offset & 31) ==  0) source.print("  ");
        source.print(data, ",");
        if((offset & 31) == 31) source.print("\n");
      });
      if(buffer.size() & 31) source.print("\n");
      source.print("};\n");
    }
  }
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Sourcery().main(arguments);
}
