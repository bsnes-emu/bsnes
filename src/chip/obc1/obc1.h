class OBC1 {
public:
struct {
  uint16 address;
  uint16 baseptr;
  uint16 shift;
} status;
  void   init();
  void   enable();
  void   power();
  void   reset();

  uint8  read (uint16 addr);
  void   write(uint16 addr, uint8 data);

  OBC1();
  ~OBC1();
};
