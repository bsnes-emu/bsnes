class SuperGameBoy : public Memory {
public:
  void enter();

  uint8_t read(unsigned addr);
  void write(unsigned addr, uint8_t data);

  void init();
  void enable();
  void power();
  void reset();

private:
  library libsgb;
  uint32_t samplebuffer[4096];

  enum { SGB1 = 0, SGB2 = 1 };
  function<bool (bool, uint8_t*, unsigned, uint8_t*, unsigned)> sgb_init;
  function<void ()> sgb_term;
  function<void ()> sgb_power;
  function<void ()> sgb_reset;
  function<uint8_t (unsigned)> sgb_read;
  function<void (unsigned, uint8_t)> sgb_write;
  function<unsigned (uint32_t*, unsigned)> sgb_run;
};

extern SuperGameBoy sgb;

