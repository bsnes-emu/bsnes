namespace phoenix {

struct pConsole : public pWidget {
  Console& console;

  void print(string text);
  void reset();
  void setBackgroundColor(Color color);
  void setForegroundColor(Color color);
  void setPrompt(string prompt);

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
};

}
