struct Terminal : Window {
  VerticalLayout layout;
  Console console;

  Terminal();
  void command(string s);
  void reset();
  void print(const string& text);
};

extern Terminal* terminal;
