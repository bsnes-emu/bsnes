struct Cheat {
  auto operator==(const Cheat& compare) const -> bool {
    return name == compare.name && code == compare.code && enable == compare.enable;
  }

  auto operator<(const Cheat& compare) const -> bool {
    return string::icompare(name, compare.name) < 0;
  }

  string name;
  string code;
  bool enable;
};

struct CheatDatabase : Window {
  CheatDatabase();
  auto findCheats() -> void;
  auto addCheats() -> void;

public:
  VerticalLayout layout{this};
    ListView cheatList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button selectAllButton{&controlLayout, Size{100, 0}};
      Button unselectAllButton{&controlLayout, Size{100, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button addCheatsButton{&controlLayout, Size{100, 0}};
};

struct CheatWindow : Window {
  CheatWindow();
  auto show(Cheat cheat = {}) -> void;
  auto doChange() -> void;
  auto doAccept() -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{40, 0}};
      LineEdit nameValue{&nameLayout, Size{~0, 0}};
    HorizontalLayout codeLayout{&layout, Size{~0, 0}};
      Label codeLabel{&codeLayout, Size{40, 0}};
      LineEdit codeValue{&codeLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{40, 0}};
      CheckLabel enableOption{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};

struct CheatEditor : TabFrameItem {
  CheatEditor(TabFrame*);
  auto refresh() -> void;
  auto addCheat(Cheat cheat) -> void;
  auto editCheat(Cheat cheat) -> void;
  auto removeCheats() -> void;
  auto loadCheats() -> void;
  auto saveCheats() -> void;
  auto synchronizeCodes() -> void;

public:
  vector<Cheat> cheats;

  VerticalLayout layout{this};
    TableView cheatList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button findCheatsButton{&controlLayout, Size{120, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button addButton{&controlLayout, Size{80, 0}};
      Button editButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};
};

struct StateWindow : Window {
  StateWindow();
  auto show(string name = {}) -> void;
  auto doChange() -> void;
  auto doAccept() -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{40, 0}};
      LineEdit nameValue{&nameLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};

struct StateManager : TabFrameItem {
  StateManager(TabFrame*);
  auto loadStates() -> void;
  auto createState(string name) -> void;
  auto modifyState(string name) -> void;
  auto removeStates() -> void;

public:
  VerticalLayout layout{this};
    TableView stateList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button loadButton{&controlLayout, Size{80, 0}};
      Button saveButton{&controlLayout, Size{80, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button addButton{&controlLayout, Size{80, 0}};
      Button editButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};
};

struct ManifestViewer : TabFrameItem {
  ManifestViewer(TabFrame*);
  auto loadManifest() -> void;

public:
  VerticalLayout layout{this};
    TextEdit manifestView{&layout, Size{~0, ~0}};
    HorizontalLayout verifiedLayout{&layout, Size{~0, 0}};
      Canvas verifiedIcon{&verifiedLayout, Size{16, 16}};
      Label verifiedLabel{&verifiedLayout, Size{~0, 0}};
};

struct ToolsWindow : Window {
  ToolsWindow();
  auto setVisible(bool visible = true) -> ToolsWindow&;
  auto show(uint index) -> void;

public:
  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      CheatEditor cheatEditor{&panel};
      StateManager stateManager{&panel};
      ManifestViewer manifestViewer{&panel};
};

extern unique_pointer<CheatDatabase> cheatDatabase;
extern unique_pointer<CheatWindow> cheatWindow;
extern unique_pointer<StateWindow> stateWindow;
extern unique_pointer<ToolsWindow> toolsWindow;
