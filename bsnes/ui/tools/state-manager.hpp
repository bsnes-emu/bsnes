struct StateManager : TopLevelWindow {
  ListBox stateList;
  Label descLabel;
  TextBox descEdit;
  Button loadButton;
  Button saveButton;
  Button eraseButton;

  void create();
  void synchronize();
  void refresh();
  void load();
  void save();
  void slotLoad();
  void slotSave();
  void slotErase();
  string slotLoadDescription(unsigned slot);
  void slotSaveDescription();

private:
  enum : unsigned {
    HeaderLength = 28,
    DescriptionLength = 512,
  };
  serializer slot[32];
};

extern StateManager stateManager;
