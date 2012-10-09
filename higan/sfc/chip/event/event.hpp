//SNES-EVENT board emulation (skeleton):
//* Campus Challenge '92
//* Powerfest '94

struct Event : Coprocessor {
  MappedRAM rom[4];
  MappedRAM ram;

  static void Enter();
  void enter();
  void init();
  void load();
  void unload();
  void power();
  void reset();

  uint8 sr(unsigned);
  void dr(unsigned, uint8 data);
  uint8 rom_read(unsigned addr);
  uint8 ram_read(unsigned addr);
  void ram_write(unsigned addr, uint8 data);

  void serialize(serializer&);

//private:
  enum class Board : unsigned { CampusChallenge92, Powerfest94 } board;
  unsigned revision;
  unsigned timer;

  string host;
  unsigned port;
  string path;
  string username;
  string password;
};

extern Event event;
