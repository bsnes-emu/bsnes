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
  auto create() -> void;
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
  auto create() -> void;
  auto show(Cheat cheat = {}) -> void;
  auto doChange() -> void;
  auto doAccept() -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{40, 0}};
      LineEdit nameValue{&nameLayout, Size{~0, 0}};
    HorizontalLayout codeLayout{&layout, Size{~0, ~0}};
      Label codeLabel{&codeLayout, Size{40, 0}};
      TextEdit codeValue{&codeLayout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{40, 0}};
      CheckLabel enableOption{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};

struct CheatEditor : TabFrameItem {
  auto create() -> void;
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
      CheckLabel enableCheats{&controlLayout, Size{0, 0}};
      Button addButton{&controlLayout, Size{80, 0}};
      Button editButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};
};

struct StateWindow : Window {
  auto create() -> void;
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

struct StateManager : TabFrameItem, Lock {
  auto create() -> void;
  auto type() const -> string;
  auto loadStates() -> void;
  auto createState(string name) -> void;
  auto modifyState(string name) -> void;
  auto removeStates() -> void;
  auto updateSelection() -> void;
  auto stateEvent(string name) -> void;

public:
  enum class SortBy : uint {
    NameAscending,
    NameDescending,
    DateAscending,
    DateDescending,
  } sortBy = SortBy::NameAscending;

  VerticalLayout layout{this};
    HorizontalLayout stateLayout{&layout, Size{~0, ~0}};
      TableView stateList{&stateLayout, Size{~0, ~0}};
      VerticalLayout previewLayout{&stateLayout, Size{0, ~0}};
        HorizontalLayout categoryLayout{&previewLayout, Size{~0, 0}};
          Label categoryLabel{&categoryLayout, Size{0, 0}};
          ComboButton categoryOption{&categoryLayout, Size{~0, 0}};
        Canvas statePreviewSeparator{&previewLayout, Size{~0, 1}};
        Label statePreviewLabel{&previewLayout, Size{~0, 0}};
        Canvas statePreview{&previewLayout, Size{256, 224}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button loadButton{&controlLayout, Size{80, 0}};
      Button saveButton{&controlLayout, Size{80, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button addButton{&controlLayout, Size{80, 0}};
      Button editButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};
};

struct ManifestViewer : TabFrameItem {
  auto create() -> void;
  auto loadManifest() -> void;
  auto selectManifest() -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout manifestLayout{&layout, Size{~0, 0}};
      Label manifestLabel{&manifestLayout, Size{0, 0}};
      ComboButton manifestOption{&manifestLayout, Size{~0, 0}};
    Canvas manifestSpacer{&layout, Size{~0, 1}};
    HorizontalLayout informationLayout{&layout, Size{~0, 0}};
      Canvas typeIcon{&informationLayout, Size{16, 16}};
      Label nameLabel{&informationLayout, Size{~0, 0}};
    #if 0 && defined(Hiro_SourceEdit)
    SourceEdit manifestView{&layout, Size{~0, ~0}};
    #else
    TextEdit manifestView{&layout, Size{~0, ~0}};
    #endif
};

struct ToolsWindow : Window {
  auto create() -> void;
  auto setVisible(bool visible = true) -> ToolsWindow&;
  auto show(uint index) -> void;

public:
  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
};

extern CheatDatabase cheatDatabase;
extern CheatWindow cheatWindow;
extern CheatEditor cheatEditor;
extern StateWindow stateWindow;
extern StateManager stateManager;
extern ManifestViewer manifestViewer;
extern ToolsWindow toolsWindow;
