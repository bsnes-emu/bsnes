class Cheat {
public:
  enum type_t {
    ProActionReplay,
    GameGenie,
  };

  struct cheat_t {
    bool     enabled;
    unsigned addr;
    uint8    data;
    string   code;
    string   desc;

    cheat_t& operator=(const cheat_t&);
    bool operator<(const cheat_t&);
  };

  static bool decode(const char *str, unsigned &addr, uint8 &data, type_t &type);
  static bool encode(string &str, unsigned addr, uint8 data, type_t type);

  inline bool enabled() const { return cheat_system_enabled; }
  inline unsigned count() const { return code.size(); }
  inline bool exists(unsigned addr) const { return bool(mask[addr >> 3] & 1 << (addr & 7)); }

  bool read(unsigned addr, uint8 &data) const;

  bool add(bool enable, const char *code, const char *desc);
  bool edit(unsigned n, bool enable, const char *code, const char *desc);
  bool get(unsigned n, cheat_t &cheat) const;
  bool remove(unsigned n);
  bool enabled(unsigned n) const;
  void enable(unsigned n);
  void disable(unsigned n);

  bool load(const char *fn);
  bool save(const char *fn) const;

  void sort();
  void clear();

  Cheat();

private:
  bool cheat_system_enabled;
  uint8 mask[0x200000];
  vector<cheat_t> code;

  void update_cheat_status();
  unsigned mirror_address(unsigned addr) const;
  void set(unsigned addr);
  void clear(unsigned addr);
};

extern Cheat cheat;
