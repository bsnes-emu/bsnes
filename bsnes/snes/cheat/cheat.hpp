struct CheatCode {
  unsigned addr;
  unsigned data;
};

class Cheat : public linear_vector<CheatCode> {
public:
  enum class Type : unsigned { ProActionReplay, GameGenie };
  uint8 *override;

  bool enabled() const;
  void enable(bool);
  void synchronize();
  uint8 read(unsigned) const;
  void init();

  Cheat();
  ~Cheat();

  static bool decode(const char*, unsigned&, unsigned&, Type&);
  static bool encode(string&, unsigned, unsigned, Type);

private:
  bool system_enabled;
  bool code_enabled;
  bool cheat_enabled;
  unsigned mirror(unsigned) const;
};

extern Cheat cheat;
