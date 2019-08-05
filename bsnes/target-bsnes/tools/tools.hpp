struct CheatCandidate {
  uint32_t address;
  uint32_t data;
  uint32_t size;
  uint32_t mode;
  uint32_t span;
};

struct CheatFinder : VerticalLayout {
  auto create() -> void;
  auto restart() -> void;
  auto refresh() -> void;
  auto eventScan() -> void;
  auto eventClear() -> void;
  auto read(uint32_t size, uint32_t address) -> uint32_t;
  auto compare(uint32_t mode, uint32_t x, uint32_t y) -> bool;

public:
  vector<CheatCandidate> candidates;

  TableView searchList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Label searchLabel{&controlLayout, Size{0, 0}};
    LineEdit searchValue{&controlLayout, Size{~0, 0}};
    ComboButton searchSize{&controlLayout, Size{0, 0}};
    ComboButton searchMode{&controlLayout, Size{0, 0}};
    ComboButton searchSpan{&controlLayout, Size{0, 0}};
    Button searchScan{&controlLayout, Size{80, 0}};
    Button searchClear{&controlLayout, Size{80, 0}};
};

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
      Button selectAllButton{&controlLayout, Size{100_sx, 0}};
      Button unselectAllButton{&controlLayout, Size{100_sx, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button addCheatsButton{&controlLayout, Size{100_sx, 0}};
};

struct CheatWindow : Window {
  auto create() -> void;
  auto show(Cheat cheat = {}) -> void;
  auto doChange() -> void;
  auto doAccept() -> void;

public:
  VerticalLayout layout{this};
    TableLayout tableLayout{&layout, Size{~0, ~0}};
      Label nameLabel{&tableLayout, Size{0, 0}};
      LineEdit nameValue{&tableLayout, Size{~0, 0}};
      Label codeLabel{&tableLayout, Size{0, 0}};
      TextEdit codeValue{&tableLayout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      CheckLabel enableOption{&controlLayout, Size{0, 0}};
      Button acceptButton{&controlLayout, Size{80_sx, 0}};
      Button cancelButton{&controlLayout, Size{80_sx, 0}};
};

struct CheatEditor : VerticalLayout {
  auto create() -> void;
  auto refresh() -> void;
  auto addCheat(Cheat cheat) -> void;
  auto editCheat(Cheat cheat) -> void;
  auto removeCheats() -> void;
  auto loadCheats() -> void;
  auto saveCheats() -> void;
  auto synchronizeCodes() -> void;

  auto decodeSNES(string& code) -> bool;
  auto decodeGB(string& code) -> bool;

public:
  vector<Cheat> cheats;
  uint64_t activateTimeout = 0;

  TableView cheatList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button findCheatsButton{&controlLayout, Size{120_sx, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    CheckLabel enableCheats{&controlLayout, Size{0, 0}};
    Button addButton{&controlLayout, Size{80_sx, 0}};
    Button editButton{&controlLayout, Size{80_sx, 0}};
    Button removeButton{&controlLayout, Size{80_sx, 0}};
};

struct StateWindow : Window {
  auto create() -> void;
  auto show(string name = {}) -> void;
  auto doChange() -> void;
  auto doAccept() -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{40_sx, 0}};
      LineEdit nameValue{&nameLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80_sx, 0}};
      Button cancelButton{&controlLayout, Size{80_sx, 0}};
};

struct StateManager : VerticalLayout, Lock {
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

  HorizontalLayout stateLayout{this, Size{~0, ~0}};
    TableView stateList{&stateLayout, Size{~0, ~0}};
    VerticalLayout previewLayout{&stateLayout, Size{0, ~0}};
      HorizontalLayout categoryLayout{&previewLayout, Size{~0, 0}};
        Label categoryLabel{&categoryLayout, Size{0, 0}};
        ComboButton categoryOption{&categoryLayout, Size{~0, 0}};
      Canvas statePreviewSeparator1{&previewLayout, Size{~0, 1}};
      Label statePreviewLabel{&previewLayout, Size{~0, 0}};
      Canvas statePreview{&previewLayout, Size{256, 224}};
      Widget stateSpacer{&previewLayout, Size{~0, ~0}};
      Canvas statePreviewSeparator2{&previewLayout, Size{~0, 1}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button loadButton{&controlLayout, Size{80_sx, 0}};
    Button saveButton{&controlLayout, Size{80_sx, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    Button addButton{&controlLayout, Size{80_sx, 0}};
    Button editButton{&controlLayout, Size{80_sx, 0}};
    Button removeButton{&controlLayout, Size{80_sx, 0}};
};

struct ManifestViewer : VerticalLayout {
  auto create() -> void;
  auto loadManifest() -> void;
  auto selectManifest() -> void;

public:
  HorizontalLayout manifestLayout{this, Size{~0, 0}};
    Label manifestLabel{&manifestLayout, Size{0, 0}};
    ComboButton manifestOption{&manifestLayout, Size{~0, 0}};
  Canvas manifestSpacer{this, Size{~0, 1}};
  HorizontalLayout informationLayout{this, Size{~0, 0}};
    Canvas typeIcon{&informationLayout, Size{16, 16}};
    Label nameLabel{&informationLayout, Size{~0, 0}};
  #if 0 && defined(Hiro_SourceEdit)
  SourceEdit manifestView{this, Size{~0, ~0}};
  #else
  TextEdit manifestView{this, Size{~0, ~0}};
  #endif
};

struct ToolsWindow : Window {
  auto create() -> void;
  auto setVisible(bool visible = true) -> ToolsWindow&;
  auto show(int index) -> void;

public:
  VerticalLayout layout{this};
    HorizontalLayout panelLayout{&layout, Size{~0, ~0}};
      ListView panelList{&panelLayout, Size{160_sx, ~0}};
      VerticalLayout panelContainer{&panelLayout, Size{~0, ~0}};
};

namespace Instances { extern Instance<CheatDatabase> cheatDatabase; }
extern CheatFinder cheatFinder;
extern CheatDatabase& cheatDatabase;
namespace Instances { extern Instance<CheatWindow> cheatWindow; }
extern CheatWindow& cheatWindow;
extern CheatEditor cheatEditor;
namespace Instances { extern Instance<StateWindow> stateWindow; }
extern StateWindow& stateWindow;
extern StateManager stateManager;
extern ManifestViewer manifestViewer;
namespace Instances { extern Instance<ToolsWindow> toolsWindow; }
extern ToolsWindow& toolsWindow;
