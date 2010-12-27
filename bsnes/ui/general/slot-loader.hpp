struct SingleSlotLoader : TopLevelWindow {
  Label baseLabel;
  TextBox basePath;
  Button baseBrowse;
  Label slotLabel;
  TextBox slotPath;
  Button slotBrowse;
  Button okButton;

  void create();
  void loadCartridgeBsxSlotted();
  void loadCartridgeBsx();
  void loadCartridgeSuperGameBoy();

  enum class Mode : unsigned { BsxSlotted, Bsx, SuperGameBoy } mode;
  void load();
};

struct DoubleSlotLoader : TopLevelWindow {
  Label baseLabel;
  TextBox basePath;
  Button baseBrowse;
  Label slotALabel;
  TextBox slotAPath;
  Button slotABrowse;
  Label slotBLabel;
  TextBox slotBPath;
  Button slotBBrowse;
  Button okButton;

  void create();
  void loadCartridgeSufamiTurbo();

  void load();
};

extern SingleSlotLoader singleSlotLoader;
extern DoubleSlotLoader doubleSlotLoader;
