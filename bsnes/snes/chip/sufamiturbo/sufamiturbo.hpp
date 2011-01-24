class SufamiTurbo {
public:
  struct Slot {
    MappedRAM rom;
    MappedRAM ram;
  } slotA, slotB;

  void enable();
  void serialize(serializer&);
};

extern SufamiTurbo sufamiturbo;
