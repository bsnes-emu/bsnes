class Interface;

enum class Input : unsigned {
  Up, Down, Left, Right, B, A, Select, Start,
};

class System {
public:
  void init(Interface*);
  void power();
  void reset();
  void run();

//private:
  Interface *interface;
};

#include <interface/interface.hpp>

extern System system;
