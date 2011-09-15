struct CheatCode {
  bool enable;
  array<unsigned> addr;
  array<unsigned> data;
  array<unsigned> comp;

  bool operator=(const string&);
  CheatCode(const string&);
  CheatCode();
};

struct Cheat : public linear_vector<CheatCode> {
  static bool decode(const string &code, unsigned &addr, unsigned &data, unsigned &comp);

  void synchronize();
  bool override[65536];
};

extern Cheat cheat;
