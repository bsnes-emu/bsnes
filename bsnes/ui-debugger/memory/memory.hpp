struct MemoryEditor : Window {
  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Label gotoLabel;
      LineEdit gotoAddress;
      ComboBox source;
      Widget spacer;
      CheckBox autoRefresh;
      Button updateButton;
    HexEdit editor;

  uint8_t read(unsigned addr);
  void write(unsigned addr, uint8_t data);
  void selectSource();
  void update();
  MemoryEditor();
};

extern MemoryEditor *memoryEditor;
