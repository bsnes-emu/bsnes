#define CHEAT_LIMIT 1024

class Cheat {
public:
enum { CT_PRO_ACTION_REPLAY, CT_GAME_GENIE };

struct CheatIndex {
  bool   enabled;
  uint32 addr;
  uint8  data;
  char   code[ 16 + 1];
  char   desc[128 + 1];
} index[CHEAT_LIMIT + 1];
bool   cheat_enabled;
uint32 cheat_count;
uint8  mask[0x200000];

  inline bool enabled() { return cheat_enabled; }
  inline uint count() { return cheat_count; }
  inline bool exists(uint32 addr) { return !!(mask[addr >> 3] & 1 << (addr & 7)); }
  inline void set(uint32 addr) { mask[addr >> 3] |= 1 << (addr & 7); }
  inline void clear(uint32 addr) { mask[addr >> 3] &= ~(1 << (addr & 7)); }

  bool  decode(char *str, uint32 &addr, uint8 &data, uint8 &type);
  bool  encode(char *str, uint32  addr, uint8  data, uint8  type);

  void  update_cheat_status();
  uint8 read(uint32 addr);
  bool  add(bool enable, char *code, char *desc);
  bool  edit(uint32 n, bool enable, char *code, char *desc);
  bool  get(uint32 n, bool &enable, uint32 &addr, uint8 &data, char *code, char *desc);
  bool  remove (uint32 n);
  bool  enabled(uint32 n);
  void  enable (uint32 n);
  void  disable(uint32 n);
  bool  load(Reader *rf);
  bool  save(Writer *wf);
  void  clear();

  Cheat();
};

extern Cheat cheat;
