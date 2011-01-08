class uPD96050 : public Coprocessor, public uPDcore, public Memory {
public:
  static void Enter();
  void enter();
  void init();
  void enable();
  void power();
  void reset();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  void serialize(serializer&);
  uPD96050();
};

extern uPD96050 upd96050;
