struct Terminal : Window {
  VerticalLayout layout;
  Console console;

  Terminal();
  void command(string s);
  void reset();
  template<typename... Args> void print(Args&&... args);
};

extern Terminal* terminal;
