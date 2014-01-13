struct Cheat {
  struct Code {
    unsigned addr;
    unsigned comp;
    unsigned data;
  };
  vector<Code> codes;
  enum : unsigned { Unused = ~0u };

  alwaysinline bool enable() const { return codes.size() > 0; }
  void reset();
  void append(unsigned addr, unsigned data);
  void append(unsigned addr, unsigned comp, unsigned data);
  optional<unsigned> find(unsigned addr, unsigned comp);
};

extern Cheat cheat;
