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
  string title() const;
};

#include "nes/nes.hpp"
#include "snes/snes.hpp"
#include "gb/gb.hpp"

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
  enum class Mode : unsigned { None, SNES, NES, GB };
  readonly<Mode> mode;

  void bindControllers();
  void setController(unsigned port, unsigned device);
  void updateDSP();

  bool cartridgeLoaded();
  void loadCartridge(Mode mode);
  bool loadCartridge(string filename);  //auto-detect system-type based on file extension
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

  bool applyPatch(CartridgePath &filepath, uint8_t *&data, unsigned &size);
  void videoRefresh(const uint32_t *input, unsigned inputPitch, unsigned width, unsigned height);

  CartridgePath base;     //base cartridge connected to emulated system
  CartridgePath slot[2];  //slot cartridges connected to base cartridge
  CartridgePath game;     //where to store resources (cheats.xml, states.bsa, ...)
  string cartridgeTitle;  //combined name of game ([base] + [slot ...])

  InterfaceCore *core;
  InterfaceNES nes;
  InterfaceSNES snes;
  InterfaceGB gb;
};

extern Interface *interface;
