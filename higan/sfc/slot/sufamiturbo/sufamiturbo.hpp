struct SufamiTurboCartridge {
  auto unload() -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;

  uint pathID = 0;
  MappedRAM rom;
  MappedRAM ram;
};

extern SufamiTurboCartridge sufamiturboA;
extern SufamiTurboCartridge sufamiturboB;
