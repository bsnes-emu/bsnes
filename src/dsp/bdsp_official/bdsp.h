class bDSP : public DSP { public:
  void   enter();
  uint8  read(uint8 addr);
  void   write(uint8 addr, uint8 data);
  void   power();
  void   reset();

  bDSP();
  ~bDSP();
};
