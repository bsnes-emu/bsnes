#include <nall/nall.hpp>
#include <nall/beat/patch.hpp>
#include <nall/emulation/super-famicom.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

struct Ananke {
  #include "configuration.cpp"

  struct Information {
    string path;      //path to selected file
    string name;      //name of selected file (inside of archive if .zip)
    string archive;   //pathname of archive
    string manifest;  //manifest from successfully applied patch
  } information;

  //archive.cpp
  vector<uint8_t> extractROM();
  vector<uint8_t> extractFile(const string &filename);

  //patch.cpp
  void applyBeatPatch(vector<uint8_t> &buffer);

  //super-famicom.cpp
  string createSuperFamicomDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest);
  string createSuperFamicomHeuristic(vector<uint8_t> &buffer);
  string openSuperFamicom(vector<uint8_t> &buffer);

  string open(string filename = "") {
    if(filename.empty()) filename = DialogWindow::fileOpen(Window::none(), config.path);
    if(filename.empty()) return "";

    information.path = dir(filename);
    information.name = notdir(filename);
    config.path = information.path;  //remember last used directory

    vector<uint8_t> buffer;
    if(filename.endswith(".zip")) {
      information.archive = filename;
      buffer = extractROM();
    } else {
      buffer = file::read(filename);
    }
    if(buffer.size() == 0) return "";  //failed to read file

    applyBeatPatch(buffer);

    if(information.name.endswith(".sfc")) return openSuperFamicom(buffer);
    return "";
  }


};

#include "archive.cpp"
#include "patch.cpp"
#include "super-famicom.cpp"

extern "C" string ananke_browse(const string &filename) {
  Ananke ananke;
  return ananke.open();
}

extern "C" string ananke_open(const string &filename) {
  Ananke ananke;
  return ananke.open(filename);
}
