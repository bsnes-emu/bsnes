class uPD7725 : public Coprocessor, public uPDcore {
public:
  static void Enter();
  void enter();
  void init();
  void enable();
  void power();
  void reset();

  void serialize(serializer&);
  uPD7725();

private:
  uint8 read(bool mode);
  void write(bool mode, uint8 data);

  friend class uPD7725SR;
  friend class uPD7725DR;
};

class uPD7725SR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

class uPD7725DR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

extern uPD7725 upd7725;
extern uPD7725SR upd7725sr;
extern uPD7725DR upd7725dr;
