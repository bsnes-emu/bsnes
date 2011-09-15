#include "nes.hpp"
#include "snes.hpp"
#include "gameboy.hpp"

struct Interface : property<Interface> {
  enum class Mode : unsigned { None, SNES, NES, GameBoy };
  readonly<Mode> mode;

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

  int16_t inputState[Scancode::Limit];
  void input_poll();
  void video_refresh();

  string baseName;  // = "/path/to/cartridge" (no extension)

private:
  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGameBoy gameBoy;
};

extern Interface *interface;
