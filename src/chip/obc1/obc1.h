class OBC1 : public Memory {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read(uint addr);
  void write(uint addr, uint8 data);

  OBC1();
  ~OBC1();

private:
  uint8 ram_read(uint addr);
  void ram_write(uint addr, uint8 data);

  struct {
    uint16 address;
    uint16 baseptr;
    uint16 shift;
  } status;
};

extern OBC1 obc1;
