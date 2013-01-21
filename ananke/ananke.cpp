#include <nall/nall.hpp>
#include <nall/beat/patch.hpp>
#include "heuristics/famicom.hpp"
#include "heuristics/super-famicom.hpp"
#include "heuristics/game-boy.hpp"
#include "heuristics/game-boy-advance.hpp"
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

namespace Database {
  #include "database/super-famicom.hpp"
  #include "database/sufami-turbo.hpp"
  #include "database/bsx-satellaview.hpp"
};

struct Ananke {
  #include "configuration.cpp"
  string libraryPath;

  Ananke();

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
  string syncFamicom(const string &pathname);

  //super-famicom.cpp
  void copySuperFamicomSaves(const string &pathname);
  string createSuperFamicomDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest);
  string createSuperFamicomHeuristic(vector<uint8_t> &buffer);
  void createSuperFamicomHeuristicFirmware(vector<uint8_t> &buffer, const string &pathname, bool firmware_appended);
  string openSuperFamicom(vector<uint8_t> &buffer);
  string syncSuperFamicom(const string &pathname);

  //sufami-turbo.cpp
  void copySufamiTurboSaves(const string &pathname);
  string createSufamiTurboDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest);
  string createSufamiTurboHeuristic(vector<uint8_t> &buffer);
  string openSufamiTurbo(vector<uint8_t> &buffer);
  string syncSufamiTurbo(const string &pathname);

  //bsx-satellaview.cpp
  string createBsxSatellaviewDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest);
  string createBsxSatellaviewHeuristic(vector<uint8_t> &buffer);
  string openBsxSatellaview(vector<uint8_t> &buffer);
  string syncBsxSatellaview(const string &pathname);

  //game-boy.cpp
  void copyGameBoySaves(const string &pathname);
  string createGameBoyHeuristic(vector<uint8_t> &buffer);
  string openGameBoy(vector<uint8_t> &buffer);
  string syncGameBoy(const string &pathname);

  //game-boy-advance.cpp
  void copyGameBoyAdvanceSaves(const string &pathname);
  string createGameBoyAdvanceHeuristic(vector<uint8_t> &buffer);
  string openGameBoyAdvance(vector<uint8_t> &buffer);
  string syncGameBoyAdvance(const string &pathname);

  static bool supported(const string &filename);
  string open(string filename = "");
  string sync(string pathname);
};

#include "resource/resource.cpp"
#include "file-dialog.cpp"
#include "archive.cpp"
#include "patch.cpp"
#include "famicom.cpp"
#include "super-famicom.cpp"
#include "sufami-turbo.cpp"
#include "bsx-satellaview.cpp"
#include "game-boy.cpp"
#include "game-boy-advance.cpp"

FileDialog *fileDialog = nullptr;

Ananke::Ananke() {
  libraryPath = string::read({configpath(), "higan/library.cfg"}).strip();
  if(libraryPath.empty()) libraryPath = {userpath(), "Emulation/"};
  if(libraryPath.endswith("/") == false) libraryPath.append("/");
}

bool Ananke::supported(const string &filename) {
  string extension = nall::extension(filename);

  if(extension == "fc" ) return true;
  if(extension == "nes") return true;
  if(extension == "sfc") return true;
  if(extension == "smc") return true;
  if(extension == "st" ) return true;
  if(extension == "bs" ) return true;
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
  if(information.name.endswith(".bs")) return openBsxSatellaview(buffer);
  if(information.name.endswith(".gb") || information.name.endswith(".gbc")) return openGameBoy(buffer);
  if(information.name.endswith(".gba")) return openGameBoyAdvance(buffer);
  return "";
}

string Ananke::sync(string pathname) {
  if(pathname.endswith(".fc/")) return syncFamicom(pathname);
  if(pathname.endswith(".sfc/")) return syncSuperFamicom(pathname);
  if(pathname.endswith(".st/")) return syncSufamiTurbo(pathname);
  if(pathname.endswith(".bs/")) return syncBsxSatellaview(pathname);
  if(pathname.endswith(".gb/")) return syncGameBoy(pathname);
  if(pathname.endswith(".gbc/")) return syncGameBoy(pathname);
  if(pathname.endswith(".gba/")) return syncGameBoyAdvance(pathname);
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

extern "C" string ananke_sync(const string &pathname) {
  Ananke ananke;
  return ananke.sync(pathname);
}
