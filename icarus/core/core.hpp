struct Icarus {
  virtual auto create(const string& pathname) -> bool {
    return directory::create(pathname);
  }

  virtual auto exists(const string& filename) -> bool {
    return file::exists(filename);
  }

  virtual auto copy(const string& target, const string& source) -> bool {
    return file::copy(target, source);
  }

  virtual auto write(const string& filename, const uint8_t* data, uint size) -> bool {
    return file::write(filename, data, size);
  }

  auto write(const string& filename, const vector<uint8_t>& buffer) -> bool {
    return write(filename, buffer.data(), buffer.size());
  }

  auto write(const string& filename, const string& text) -> bool {
    return write(filename, (const uint8_t*)text.data(), text.size());
  }

  //core.cpp
  Icarus();

  auto error() const -> string;
  auto missing() const -> vector<string>;
  auto success(string location) -> string;
  auto failure(string message) -> string;

  auto manifest(string location) -> string;
  auto import(string location) -> string;

  auto concatenate(vector<uint8_t>& output, string location) -> void;

  //famicom.cpp
  auto famicomManifest(string location) -> string;
  auto famicomManifest(vector<uint8_t>& buffer, string location) -> string;
  auto famicomImport(vector<uint8_t>& buffer, string location) -> string;

  //super-famicom.cpp
  auto superFamicomManifest(string location) -> string;
  auto superFamicomManifest(vector<uint8_t>& buffer, string location) -> string;
  auto superFamicomImport(vector<uint8_t>& buffer, string location) -> string;

  //master-system.cpp
  auto masterSystemManifest(string location) -> string;
  auto masterSystemManifest(vector<uint8_t>& buffer, string location) -> string;
  auto masterSystemImport(vector<uint8_t>& buffer, string location) -> string;

  //mega-drive.cpp
  auto megaDriveManifest(string location) -> string;
  auto megaDriveManifest(vector<uint8_t>& buffer, string location) -> string;
  auto megaDriveImport(vector<uint8_t>& buffer, string location) -> string;

  //pc-engine.cpp
  auto pcEngineManifest(string location) -> string;
  auto pcEngineManifest(vector<uint8_t>& buffer, string location) -> string;
  auto pcEngineImport(vector<uint8_t>& buffer, string location) -> string;

  //supergrafx.cpp
  auto superGrafxManifest(string location) -> string;
  auto superGrafxManifest(vector<uint8_t>& buffer, string location) -> string;
  auto superGrafxImport(vector<uint8_t>& buffer, string location) -> string;

  //game-boy.cpp
  auto gameBoyManifest(string location) -> string;
  auto gameBoyManifest(vector<uint8_t>& buffer, string location) -> string;
  auto gameBoyImport(vector<uint8_t>& buffer, string location) -> string;

  //game-boy-color.cpp
  auto gameBoyColorManifest(string location) -> string;
  auto gameBoyColorManifest(vector<uint8_t>& buffer, string location) -> string;
  auto gameBoyColorImport(vector<uint8_t>& buffer, string location) -> string;

  //game-boy-advance.cpp
  auto gameBoyAdvanceManifest(string location) -> string;
  auto gameBoyAdvanceManifest(vector<uint8_t>& buffer, string location) -> string;
  auto gameBoyAdvanceImport(vector<uint8_t>& buffer, string location) -> string;

  //game-gear.cpp
  auto gameGearManifest(string location) -> string;
  auto gameGearManifest(vector<uint8_t>& buffer, string location) -> string;
  auto gameGearImport(vector<uint8_t>& buffer, string location) -> string;

  //wonderswan.cpp
  auto wonderSwanManifest(string location) -> string;
  auto wonderSwanManifest(vector<uint8_t>& buffer, string location) -> string;
  auto wonderSwanImport(vector<uint8_t>& buffer, string location) -> string;

  //wonderswan-color.cpp
  auto wonderSwanColorManifest(string location) -> string;
  auto wonderSwanColorManifest(vector<uint8_t>& buffer, string location) -> string;
  auto wonderSwanColorImport(vector<uint8_t>& buffer, string location) -> string;

  //pocket-challenge-v2.cpp
  auto pocketChallengeV2Manifest(string location) -> string;
  auto pocketChallengeV2Manifest(vector<uint8_t>& buffer, string location) -> string;
  auto pocketChallengeV2Import(vector<uint8_t>& buffer, string location) -> string;

  //bs-memory.cpp
  auto bsMemoryManifest(string location) -> string;
  auto bsMemoryManifest(vector<uint8_t>& buffer, string location) -> string;
  auto bsMemoryImport(vector<uint8_t>& buffer, string location) -> string;

  //sufami-turbo.cpp
  auto sufamiTurboManifest(string location) -> string;
  auto sufamiTurboManifest(vector<uint8_t>& buffer, string location) -> string;
  auto sufamiTurboImport(vector<uint8_t>& buffer, string location) -> string;

private:
  string errorMessage;
  vector<string> missingFiles;
};

namespace Database {
  Markup::Node Famicom;
  Markup::Node SuperFamicom;
  Markup::Node MasterSystem;
  Markup::Node MegaDrive;
  Markup::Node PCEngine;
  Markup::Node SuperGrafx;
  Markup::Node GameBoy;
  Markup::Node GameBoyColor;
  Markup::Node GameBoyAdvance;
  Markup::Node GameGear;
  Markup::Node WonderSwan;
  Markup::Node WonderSwanColor;
  Markup::Node PocketChallengeV2;
  Markup::Node BSMemory;
  Markup::Node SufamiTurbo;
};
