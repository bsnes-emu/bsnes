struct System {
  Interface *interface;

  void run();
  void power();
  void reset();

  void init(Interface *interface);
  void term();
};

extern System system;
