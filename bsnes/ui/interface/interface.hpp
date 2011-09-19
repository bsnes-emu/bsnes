#include "nes.hpp"
#include "snes.hpp"
#include "gameboy.hpp"

struct Interface : property<Interface> {
  enum class Mode : unsigned { None, SNES, NES, GameBoy };
  readonly<Mode> mode;

  void bindControllers();
  void setController(unsigned port, unsigned device);

  bool cartridgeLoaded();
  void loadCartridge(Mode mode);
  bool loadCartridge(const string &filename);  //auto-detect system-type based on file extension
  void unloadCartridge();

  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  bool saveState(const string &filename);
  bool loadState(const string &filename);
  void setCheatCodes(const lstring &list = lstring{});

  Interface();

  void videoRefresh();

  string baseName;  // = "/path/to/cartridge" (no extension)

  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGameBoy gameBoy;
};

extern Interface *interface;
