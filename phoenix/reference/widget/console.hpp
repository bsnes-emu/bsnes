namespace phoenix {

struct pConsole : public pWidget {
  Console& console;

  void print(string text);
  void reset();

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
};

}
