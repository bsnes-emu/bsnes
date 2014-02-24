struct Terminal : Window {
  struct Alias {
    string name;
    string mapping;
  };

  struct Hotkey {
    string name;
    string mapping;
  };

  Terminal();
  void load();
  void unload();

  void command(string s);
  void echoAliases();
  void echoHotkeys();
  void inputEvent(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);
  void reset();
  void print(const string& text);
  void setColors();

  VerticalLayout layout;
  Console console;

  vector<Alias> aliases;
  vector<Hotkey> hotkeys;
};

extern Terminal* terminal;
