#include "palette.hpp"

#include "nes.hpp"
#include "snes.hpp"
#include "gameboy.hpp"

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

  Interface();

  void videoRefresh(const uint16_t *input, unsigned inputPitch, unsigned width, unsigned height);

  string baseName;  // = "/path/to/cartridge" (no extension)
  lstring slotName;

  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGameBoy gameBoy;
};

extern Interface *interface;
