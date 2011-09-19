#include "nes.hpp"
#include "snes.hpp"
#include "gameboy.hpp"

struct Interface : property<Interface> {
  enum class Mode : unsigned { None, SNES, NES, GameBoy };
  readonly<Mode> mode;

  void bindControllers();
  void setController(unsigned port, unsigned device);

  bool loaded();

  bool loadCartridge(const string &filename);
  bool loadCartridgeNES(const string &filename);
  bool loadCartridgeSNES(const string &filename);
  bool loadCartridgeGameBoy(const string &filename);

  void unloadCartridge();
  void unloadCartridgeNES();
  void unloadCartridgeSNES();
  void unloadCartridgeGameBoy();

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

private:
  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGameBoy gameBoy;
};

extern Interface *interface;
