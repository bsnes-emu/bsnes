class Cheat {
public:
  enum { CheatLimit = 1024 };

  enum Type {
    ProActionReplay,
    GameGenie,
  };

  struct CheatIndex {
    bool enabled;
    uint32 addr;
    uint8 data;
    char code[ 16 + 1];
    char desc[128 + 1];
  } index[CheatLimit + 1];

  bool cheat_enabled;
  uint32 cheat_count;
  uint8 mask[0x200000];

  inline bool enabled() { return cheat_enabled; }
  inline uint count() { return cheat_count; }
  inline bool exists(uint32 addr) { return bool(mask[addr >> 3] & 1 << (addr & 7)); }

  bool decode(char *str, uint32 &addr, uint8 &data, uint8 &type);
  bool encode(char *str, uint32  addr, uint8  data, uint8  type);

  bool read(uint32 addr, uint8 &data);

  void update_cheat_status();
  bool add(bool enable, char *code, char *desc);
  bool edit(uint32 n, bool enable, char *code, char *desc);
  bool get(uint32 n, bool &enable, uint32 &addr, uint8 &data, char *code, char *desc);
  bool remove(uint32 n);
  bool enabled(uint32 n);
  void enable(uint32 n);
  void disable(uint32 n);
  bool load(const char *fn);
  bool save(const char *fn);
  void clear();

  Cheat();

private:
  uint mirror_address(uint addr);
  void set(uint32 addr);
  void clear(uint32 addr);
};

extern Cheat cheat;
