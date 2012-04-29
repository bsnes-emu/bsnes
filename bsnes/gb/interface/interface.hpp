struct Interface : Emulator::Interface {
  virtual void lcdScanline() {}
  virtual void joypWrite(bool p15, bool p14) {}
};

extern Interface *interface;
