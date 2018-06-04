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
  auto show(TableViewItem item = {}) -> void;
  auto doChange() -> void;
  auto doAccept() -> void;

public:
  TableViewItem item;

  VerticalLayout layout{this};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{40, 0}};
      LineEdit nameValue{&nameLayout, Size{~0, 0}};
    HorizontalLayout codeLayout{&layout, Size{~0, 0}};
      Label codeLabel{&codeLayout, Size{40, 0}};
      LineEdit codeValue{&codeLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{40, 0}};
      CheckLabel enabledOption{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};

struct CheatEditor : TabFrameItem {
  CheatEditor(TabFrame*);
  auto swap(uint x, uint y) -> void;
  auto synchronizeCodes() -> void;
  auto addCheat(string name, string code, bool enabled = false) -> void;
  auto loadCheats() -> void;
  auto saveCheats() -> void;

public:
  VerticalLayout layout{this};
    TableView cheatList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button upButton{&controlLayout, Size{0, 0}};
      Button downButton{&controlLayout, Size{0, 0}};
      Button findCheatsButton{&controlLayout, Size{120, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button resetButton{&controlLayout, Size{80, 0}};
      Button appendButton{&controlLayout, Size{80, 0}};
      Button modifyButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};
};

/*
struct CheatEditor : TabFrameItem {
  enum : uint { Slots = 128 };

  CheatEditor(TabFrame*);
  auto doChangeSelected() -> void;
  auto doModify() -> void;
  auto doRefresh() -> void;
  auto doReset(bool force = false) -> void;
  auto doErase() -> void;
  auto synchronizeCodes() -> void;
  auto addCode(bool enabled, string code, string description) -> bool;
  auto loadCheats() -> void;
  auto saveCheats() -> void;

public:
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
*/

struct ToolsWindow : Window {
  ToolsWindow();
  auto setVisible(bool visible = true) -> ToolsWindow&;
  auto show(uint index) -> void;

public:
  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      CheatEditor cheatEditor{&panel};
};

extern unique_pointer<CheatDatabase> cheatDatabase;
extern unique_pointer<CheatWindow> cheatWindow;
extern unique_pointer<ToolsWindow> toolsWindow;
