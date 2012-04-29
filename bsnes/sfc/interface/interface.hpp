struct Interface : Emulator::Interface {
  virtual string path(Cartridge::Slot slot, const string &hint) = 0;
  virtual void message(const string &text);
};

extern Interface *interface;
