struct SufamiTurboCartridge {
  MappedRAM rom;
  MappedRAM ram;

  void load();
  void unload();
  void serialize(serializer&);
};

extern SufamiTurboCartridge sufamiturboA;
extern SufamiTurboCartridge sufamiturboB;
