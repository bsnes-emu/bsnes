#ifndef FC_HPP
namespace Famicom {
#endif

struct ID {
  enum : unsigned {
    ROM,
    RAM,
  };
};

struct Interface : Emulator::Interface {
  bool loaded();
  void load(unsigned id, const stream &stream, const string &markup = "");
  void save(unsigned id, const stream &stream);
  void unload();

  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  void updatePalette();

  Interface();

private:
  Port::Device controller();
};

extern Interface *interface;

#ifndef FC_HPP
}
#endif
