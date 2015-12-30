struct Cheat {
  enum : unsigned { Unused = ~0u };

  alwaysinline auto enable() const -> bool { return codes.size() > 0; }

  auto reset() -> void;
  auto append(unsigned addr, unsigned data) -> void;
  auto append(unsigned addr, unsigned comp, unsigned data) -> void;
  auto find(unsigned addr, unsigned comp) -> maybe<unsigned>;

  struct Code {
    unsigned addr;
    unsigned comp;
    unsigned data;
  };
  vector<Code> codes;
};

extern Cheat cheat;
