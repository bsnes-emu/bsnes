#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

struct Ananke {
  struct Configuration : configuration {
    string path;

    Configuration() {
      append(path = userpath(), "Path");
      directory::create({configpath(), "ananke/"});
      load({configpath(), "ananke/settings.cfg"});
    }

    ~Configuration() {
      save({configpath(), "ananke/settings.cfg"});
    }
  } config;

  string createSuperFamicomDatabase(const string &filename, Markup::Node &document, const string &manifest) {
    string pathname = {
      userpath(), "Emulation/Super Famicom/",
      document["release/information/name"].text(),
      " (", document["release/information/region"].text(), ")",
      " (", document["release/information/revision"].text(), ")",
      ".sfc/"
    };
    directory::create(pathname);

    file::write({pathname, "manifest.bml"}, (const uint8_t*)(const char*)manifest, manifest.length());
    auto buffer = file::read(filename);
    unsigned offset = 0;

    for(auto &node : document["release/information/configuration"]) {
      if(node.name != "rom") continue;
      string name = node["name"].text();
      unsigned size = node["size"].decimal();
      if(file::exists({pathname, name}) == false) {
        file::write({pathname, name}, buffer.data() + offset, size);
      }
      offset += size;
    }

    return pathname;
  }

  string openSuperFamicom(const string &filename) {
    string sha256 = file::sha256(filename);

    string databaseText = string::read({configpath(), "ananke/database/Super Famicom.bml"}).rtrim("\n");
    lstring databaseItem = databaseText.split("\n\n");

    for(auto &item : databaseItem) {
      item.append("\n");
      auto document = Markup::Document(item);

      if(document["release/information/sha256"].text() == sha256) {
        return createSuperFamicomDatabase(filename, document, item);
      }
    }

    return "";
  }

  string open(string filename = "") {
    if(filename.empty()) filename = DialogWindow::fileOpen(Window::none(), config.path);
    if(filename.empty()) return "";
    config.path = dir(filename);

    if(filename.endswith(".sfc")) return openSuperFamicom(filename);
    return "";
  }
};

extern "C" string ananke_browse(const string &filename) {
  Ananke ananke;
  return ananke.open();
}

extern "C" string ananke_open(const string &filename) {
  Ananke ananke;
  return ananke.open(filename);
}
