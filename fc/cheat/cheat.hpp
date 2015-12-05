struct Cheat {
  struct Code {
    uint addr;
    uint comp;
    uint data;
  };
  vector<Code> codes;
  enum : uint { Unused = ~0u };

  alwaysinline auto enable() const -> bool { return codes.size() > 0; }
  auto reset() -> void;
  auto append(uint addr, uint data) -> void;
  auto append(uint addr, uint comp, uint data) -> void;
  auto find(uint addr, uint comp) -> maybe<uint>;
};

extern Cheat cheat;
