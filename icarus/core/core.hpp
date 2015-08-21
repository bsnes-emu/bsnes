struct Icarus {
  //core.cpp
  auto error() const -> string;
  auto success() -> bool;
  auto failure(const string& message) -> bool;

  auto manifest(string location) -> string;
  auto import(string location) -> bool;

  auto concatenate(vector<uint8_t>& output, const string& location) -> void;

  //famicom.cpp
  auto famicomManifest(const string& location) -> string;
  auto famicomManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto famicomImport(vector<uint8_t>& buffer, const string& location) -> bool;

  //super-famicom.cpp
  auto superFamicomManifest(const string& location) -> string;
  auto superFamicomManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto superFamicomImport(vector<uint8_t>& buffer, const string& location) -> bool;
  auto superFamicomImportScanManifest(vector<Markup::Node>& roms, Markup::Node node) -> void;

  //game-boy.cpp
  auto gameBoyManifest(const string& location) -> string;
  auto gameBoyManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto gameBoyImport(vector<uint8_t>& buffer, const string& location) -> bool;

  //game-boy-color.cpp
  auto gameBoyColorManifest(const string& location) -> string;
  auto gameBoyColorManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto gameBoyColorImport(vector<uint8_t>& buffer, const string& location) -> bool;

  //game-boy-advance.cpp
  auto gameBoyAdvanceManifest(const string& location) -> string;
  auto gameBoyAdvanceManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto gameBoyAdvanceImport(vector<uint8_t>& buffer, const string& location) -> bool;

  //bsx-satellaview.cpp
  auto bsxSatellaviewManifest(const string& location) -> string;
  auto bsxSatellaviewManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto bsxSatellaviewImport(vector<uint8_t>& buffer, const string& location) -> bool;

  //sufami-turbo.cpp
  auto sufamiTurboManifest(const string& location) -> string;
  auto sufamiTurboManifest(vector<uint8_t>& buffer, const string& location) -> string;
  auto sufamiTurboImport(vector<uint8_t>& buffer, const string& location) -> bool;

private:
  string errorMessage;
};
