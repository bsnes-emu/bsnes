struct MemoryEditor : Window {
  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Label gotoLabel;
      LineEdit gotoAddress;
      ComboBox source;
      Button exportMemory;
      Widget spacer;
      CheckBox autoUpdate;
      Button update;
    HexEdit editor;

  uint8_t read(unsigned addr);
  void write(unsigned addr, uint8_t data);
  void selectSource();
  void exportMemoryToDisk();
  void updateView();
  MemoryEditor();
};

extern MemoryEditor *memoryEditor;
