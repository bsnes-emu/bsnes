struct SufamiTurboCartridge {
  auto load() -> void;
  auto unload() -> void;
  auto serialize(serializer&) -> void;

  MappedRAM rom;
  MappedRAM ram;
};

extern SufamiTurboCartridge sufamiturboA;
extern SufamiTurboCartridge sufamiturboB;
