#include <nall/nall.hpp>
#include <nall/beat/patch.hpp>
#include <nall/emulation/famicom.hpp>
#include <nall/emulation/super-famicom.hpp>
#include <nall/emulation/game-boy.hpp>
#include <nall/emulation/game-boy-advance.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

namespace Database {
  #include "database/super-famicom.hpp"
  #include "database/sufami-turbo.hpp"
};

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

  //famicom.cpp
  void copyFamicomSaves(const string &pathname);
  string createFamicomHeuristic(vector<uint8_t> &buffer);
  string openFamicom(vector<uint8_t> &buffer);

  //super-famicom.cpp
  void copySuperFamicomSaves(const string &pathname);
  string createSuperFamicomDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest);
  string createSuperFamicomHeuristic(vector<uint8_t> &buffer);
  void createSuperFamicomHeuristicFirmware(vector<uint8_t> &buffer, const string &pathname, bool firmware_appended);
  string openSuperFamicom(vector<uint8_t> &buffer);

  //sufami-turbo.cpp
  void copySufamiTurboSaves(const string &pathname);
  string createSufamiTurboDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest);
  string createSufamiTurboHeuristic(vector<uint8_t> &buffer);
  string openSufamiTurbo(vector<uint8_t> &buffer);

  //game-boy.cpp
  void copyGameBoySaves(const string &pathname);
  string createGameBoyHeuristic(vector<uint8_t> &buffer);
  string openGameBoy(vector<uint8_t> &buffer);

  //game-boy-advance.cpp
  void copyGameBoyAdvanceSaves(const string &pathname);
  string createGameBoyAdvanceHeuristic(vector<uint8_t> &buffer);
  string openGameBoyAdvance(vector<uint8_t> &buffer);

  static bool supported(const string &filename);
  string open(string filename = "");
};

#include "resource/resource.cpp"
#include "file-dialog.cpp"
#include "archive.cpp"
#include "patch.cpp"
#include "famicom.cpp"
#include "super-famicom.cpp"
#include "sufami-turbo.cpp"
#include "game-boy.cpp"
#include "game-boy-advance.cpp"

FileDialog *fileDialog = nullptr;

bool Ananke::supported(const string &filename) {
  string extension = nall::extension(filename);

  if(extension == "fc" ) return true;
  if(extension == "nes") return true;
  if(extension == "sfc") return true;
  if(extension == "smc") return true;
  if(extension == "bs" ) return true;
  if(extension == "st" ) return true;
  if(extension == "gb" ) return true;
  if(extension == "gbc") return true;
  if(extension == "gba") return true;
  if(extension == "zip") return true;

  return false;
}

string Ananke::open(string filename) {
  if(filename.empty()) {
    if(!fileDialog) fileDialog = new FileDialog;
    fileDialog->setPath(config.path);
    filename = fileDialog->open();
  }

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

  if(information.name.endswith(".fc") || information.name.endswith(".nes")) return openFamicom(buffer);
  if(information.name.endswith(".sfc") || information.name.endswith(".smc")) return openSuperFamicom(buffer);
  if(information.name.endswith(".st")) return openSufamiTurbo(buffer);
  if(information.name.endswith(".gb") || information.name.endswith(".gbc")) return openGameBoy(buffer);
  if(information.name.endswith(".gba")) return openGameBoyAdvance(buffer);
  return "";
}

extern "C" string ananke_browse(const string &filename) {
  Ananke ananke;
  return ananke.open();
}

extern "C" string ananke_open(const string &filename) {
  Ananke ananke;
  return ananke.open(filename);
}
