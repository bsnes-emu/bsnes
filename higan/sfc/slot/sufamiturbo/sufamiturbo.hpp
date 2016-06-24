struct SufamiTurboCartridge {
  auto load() -> void;
  auto unload() -> void;
  auto serialize(serializer&) -> void;

  uint pathID = 0;
  MappedRAM rom;
  MappedRAM ram;
};

extern SufamiTurboCartridge sufamiturboA;
extern SufamiTurboCartridge sufamiturboB;
