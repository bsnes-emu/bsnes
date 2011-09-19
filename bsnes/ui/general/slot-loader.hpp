struct SlotLoaderPath : HorizontalLayout {
  Label label;
  LineEdit path;
  Button browse;

  string name;
  lstring filter;

  SlotLoaderPath();
};

struct SlotLoader : Window {
  VerticalLayout layout;
  SlotLoaderPath base;
  SlotLoaderPath slot[2];
  HorizontalLayout controlLayout;
    Widget spacer;
    Button loadButton;

  void synchronize();
  void loadSatellaviewSlotted();
  void loadSatellaview();
  void loadSufamiTurbo();
  void loadSuperGameBoy();

  SlotLoader();
};

extern SlotLoader *slotLoader;
