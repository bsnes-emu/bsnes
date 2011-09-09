struct SingleSlotLoader : TopLevelWindow {
  VerticalLayout layout;
  HorizontalLayout baseLayout;
  Label baseLabel;
  LineEdit basePath;
  Button baseBrowse;
  HorizontalLayout slotLayout;
  Label slotLabel;
  LineEdit slotPath;
  Button slotBrowse;
  HorizontalLayout controlLayout;
  Label spacer;
  Button okButton;

  void create();
  void loadCartridgeBsxSlotted();
  void loadCartridgeBsx();
  void loadCartridgeSuperGameBoy();

  enum class Mode : unsigned { BsxSlotted, Bsx, SuperGameBoy } mode;
  void load();
};

struct DoubleSlotLoader : TopLevelWindow {
  VerticalLayout layout;
  HorizontalLayout baseLayout;
  Label baseLabel;
  LineEdit basePath;
  Button baseBrowse;
  HorizontalLayout slotALayout;
  Label slotALabel;
  LineEdit slotAPath;
  Button slotABrowse;
  HorizontalLayout slotBLayout;
  Label slotBLabel;
  LineEdit slotBPath;
  Button slotBBrowse;
  HorizontalLayout controlLayout;
  Label spacer;
  Button okButton;

  void create();
  void loadCartridgeSufamiTurbo();

  void load();
};

extern SingleSlotLoader singleSlotLoader;
extern DoubleSlotLoader doubleSlotLoader;
