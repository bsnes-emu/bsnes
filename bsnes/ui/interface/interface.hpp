#include "palette.hpp"

struct InterfaceCore {
  virtual void power() = 0;
  virtual void reset() = 0;
  virtual void run() = 0;

  virtual serializer serialize() = 0;
  virtual bool unserialize(serializer&) = 0;
};

struct CartridgePath {
  bool folder;
  string name;
  string filename(const string &folderName, const string &fileName) const;
};

#include "nes/nes.hpp"
#include "snes/snes.hpp"
#include "gameboy/gameboy.hpp"

struct Filter : public library {
  function<void (unsigned&, unsigned&)> dl_size;
  function<void (uint32_t*, unsigned, const uint32_t*, unsigned, unsigned, unsigned)> dl_render;
  void render(const uint32_t*, unsigned, unsigned, unsigned);
  Filter();
  ~Filter();

  uint32_t *data;
  unsigned pitch;
  unsigned width;
  unsigned height;
};

extern Filter filter;

struct Interface : property<Interface> {
  enum class Mode : unsigned { None, SNES, NES, GameBoy };
  readonly<Mode> mode;

  void bindControllers();
  void setController(unsigned port, unsigned device);
  void updateDSP();

  bool cartridgeLoaded();
  void loadCartridge(Mode mode);
  bool loadCartridge(const string &filename);  //auto-detect system-type based on file extension
  void unloadCartridge();

  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  bool saveState(unsigned slot);
  bool loadState(unsigned slot);
  void setCheatCodes(const lstring &list = lstring{});
  string sha256();

  Interface();

  bool applyPatch(const string &filename, uint8_t *&data, unsigned &size);
  void videoRefresh(const uint32_t *input, unsigned inputPitch, unsigned width, unsigned height);

  CartridgePath base;
  vector<CartridgePath> slot;

  //deprecated
  string baseName;  // = "/path/to/cartridge" (no extension)

  InterfaceCore *core;
  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGameBoy gameBoy;
};

extern Interface *interface;
