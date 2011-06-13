class Link : public Coprocessor, public library {
public:
  string program;

  void init();
  void load();
  void unload();
  void power();
  void reset();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

private:
  function<void ()> link_power;
  function<void ()> link_reset;
  function<uint8 (unsigned)> link_read;
  function<void (unsigned, uint8)> link_write;
};

extern Link link;
