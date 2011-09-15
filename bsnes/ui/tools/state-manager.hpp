struct StateManager : Window {
  VerticalLayout layout;
  ListView stateList;
  HorizontalLayout descLayout;
    Label descLabel;
    LineEdit descEdit;
  HorizontalLayout controlLayout;
    Widget spacer;
    Button loadButton;
    Button saveButton;
    Button eraseButton;

  void synchronize();
  void refresh();

  bool load(const string &filename, unsigned revision);
  bool save(const string &filename, unsigned revision);

  void slotLoad();
  void slotSave();
  void slotErase();

  string slotLoadDescription(unsigned n);
  void slotSaveDescription();

  StateManager();

private:
  enum : unsigned {
    //these valus are standardized across all emulated platforms:
    //{ uint32 signature, version, checksum; char description[512]; ... }
    HeaderLength = 12,
    DescriptionLength = 512,
  };
  serializer slot[32];
};

extern StateManager *stateManager;
