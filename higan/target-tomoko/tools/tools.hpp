struct CheatDatabase : Window {
  CheatDatabase();
  auto findCodes() -> void;
  auto addCodes() -> void;

  vector<string> codes;

  VerticalLayout layout{this};
    ListView cheatList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button selectAllButton{&controlLayout, Size{100, 0}};
      Button unselectAllButton{&controlLayout, Size{100, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button addCodesButton{&controlLayout, Size{100, 0}};
};

struct CheatEditor : TabFrameItem {
  enum : uint { Slots = 128 };

  CheatEditor(TabFrame*);
  auto doChangeSelected() -> void;
  auto doModify() -> void;
  auto doRefresh() -> void;
  auto doReset(bool force = false) -> void;
  auto doErase() -> void;
  auto synchronizeCodes() -> void;
  auto addCode(const string& code, const string& description, bool enabled = false) -> bool;
  auto loadCheats() -> void;
  auto saveCheats() -> void;

  struct Cheat {
    bool enabled = false;
    string code;
    string description;
  };
  Cheat cheats[Slots];

  VerticalLayout layout{this};
    TableView cheatList{&layout, Size{~0, ~0}};
    HorizontalLayout codeLayout{&layout, Size{~0, 0}};
      Label codeLabel{&codeLayout, Size{70, 0}};
      LineEdit codeValue{&codeLayout, Size{~0, 0}};
    HorizontalLayout descriptionLayout{&layout, Size{~0, 0}};
      Label descriptionLabel{&descriptionLayout, Size{70, 0}};
      LineEdit descriptionValue{&descriptionLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button findCodesButton{&controlLayout, Size{120, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button eraseButton{&controlLayout, Size{80, 0}};
};

struct StateManager : TabFrameItem {
  enum : uint { Slots = 32 };

  StateManager(TabFrame*);
  auto doChangeSelected() -> void;
  auto doRefresh() -> void;
  auto doUpdateControls() -> void;
  auto doChangeDescription() -> void;
  auto doLoad() -> void;
  auto doSave() -> void;
  auto doReset() -> void;
  auto doErase() -> void;

  VerticalLayout layout{this};
    TableView stateList{&layout, Size{~0, ~0}};
    HorizontalLayout descriptionLayout{&layout, Size{~0, 0}};
      Label descriptionLabel{&descriptionLayout, Size{70, 0}};
      LineEdit descriptionValue{&descriptionLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button saveButton{&controlLayout, Size{80, 0}};
      Button loadButton{&controlLayout, Size{80, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button eraseButton{&controlLayout, Size{80, 0}};
};

struct ManifestViewer : TabFrameItem {
  ManifestViewer(TabFrame*);
  auto doRefresh() -> void;

  VerticalLayout layout{this};
    TextEdit manifestView{&layout, Size{~0, ~0}};
};

struct ToolsManager : Window {
  ToolsManager();
  auto show(uint tool) -> void;

  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      CheatEditor cheatEditor{&panel};
      StateManager stateManager{&panel};
      ManifestViewer manifestViewer{&panel};
};

extern unique_pointer<CheatDatabase> cheatDatabase;
extern unique_pointer<ToolsManager> toolsManager;
