struct StateManager : Window {
  VerticalLayout layout;
  ListView stateList;
  HorizontalLayout descLayout;
    Label descLabel;
    LineEdit descEdit;
  HorizontalLayout controlLayout;
    Button saveButton;
    Button loadButton;
    Widget spacer;
    Button resetButton;
    Button eraseButton;

  void reset();
  bool load(string filename, unsigned revision);
  bool save(string filename, unsigned revision);

  void slotLoad();
  void slotSave();
  void slotErase();
  string slotLoadDescription(unsigned id);
  void slotSaveDescription();

  void refresh();
  void synchronize();
  StateManager();

private:
  enum : unsigned {
    Slots = 32,
    HeaderLength = 72,
    DescriptionLength = 512,
  };
  serializer slot[Slots];
};

extern StateManager* stateManager;
