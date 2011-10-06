#include "palette.hpp"

#include "nes.hpp"
#include "snes.hpp"
#include "gameboy.hpp"

struct Filter : public library {
  function<void (unsigned&, unsigned&)> dl_size;
  function<void (uint16_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned)> dl_render;
  void render(const uint16_t*, unsigned, unsigned, unsigned);
  Filter();
  ~Filter();

  uint16_t *data;
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

  bool loadFile(const string &filename, uint8_t *&data, unsigned &size);
  void videoRefresh(const uint16_t *input, unsigned inputPitch, unsigned width, unsigned height);

  string baseName;  // = "/path/to/cartridge" (no extension)
  lstring slotName;

  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGameBoy gameBoy;
};

extern Interface *interface;
