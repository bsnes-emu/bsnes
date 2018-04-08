struct Memory {
  string type;
  boolean battery;
  string size;
  string content;
  string manufacturer;
  string architecture;
  string identifier;
};

struct Oscillator {
  string frequency;
};

//variant meta-class
struct Component {
  enum class Type : uint {
    Memory,
    Oscillator,
  } type;
  Memory memory;
  Oscillator oscillator;
};

struct Game {
  string sha256;
  string region;
  string revision;
  string board;
  string name;
  string label;
  string note;
  vector<Component> components;
};

struct ListWindow : Window {
  ListWindow();
  auto quit() -> void;
  auto reloadList() -> void;
  auto updateWindow() -> void;
  auto newDatabase() -> void;
  auto loadDatabase(string) -> void;
  auto saveDatabase(string) -> void;
  auto appendGame(Game) -> void;
  auto modifyGame(Game) -> void;
  auto removeGame() -> void;

private:
  bool modified = false;
  vector<Game> games;
  string location;

  MenuBar menuBar{this};
    Menu fileMenu{&menuBar};
      MenuItem newAction{&fileMenu};
      MenuItem openAction{&fileMenu};
      MenuItem saveAction{&fileMenu};
      MenuItem saveAsAction{&fileMenu};
      MenuSeparator quitSeparator{&fileMenu};
      MenuItem quitAction{&fileMenu};
    Menu helpMenu{&menuBar};
      MenuItem aboutAction{&helpMenu};

  HorizontalLayout layout{this};
    TableView gameList{&layout, Size{~0, ~0}};
    VerticalLayout controlLayout{&layout, Size{80, ~0}};
      Button appendButton{&controlLayout, Size{~0, 0}};
      Button modifyButton{&controlLayout, Size{~0, 0}};
      Button removeButton{&controlLayout, Size{~0, 0}};
};

struct GameWindow : Window {
  GameWindow();
  auto show(Game = {}) -> void;
  auto accept() -> void;
  auto cancel() -> void;
  auto reloadList() -> void;
  auto updateWindow() -> void;
  auto appendComponent(Component) -> void;
  auto modifyComponent(Component) -> void;
  auto removeComponent() -> void;

private:
  bool modified = false;
  bool create = true;
  Game game;

  VerticalLayout layout{this};
    HorizontalLayout hashLayout{&layout, Size{~0, 0}};
      Label hashLabel{&hashLayout, Size{50, 0}};
      LineEdit hashEdit{&hashLayout, Size{~0, 0}};
    HorizontalLayout infoLayout{&layout, Size{~0, 0}};
      Label regionLabel{&infoLayout, Size{50, 0}};
      LineEdit regionEdit{&infoLayout, Size{~0, 0}};
      Label revisionLabel{&infoLayout, Size{0, 0}};
      LineEdit revisionEdit{&infoLayout, Size{~0, 0}};
      Label boardLabel{&infoLayout, Size{0, 0}};
      LineEdit boardEdit{&infoLayout, Size{~0, 0}, 0};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{50, 0}};
      LineEdit nameEdit{&nameLayout, Size{~0, 0}};
    HorizontalLayout labelLayout{&layout, Size{~0, 0}};
      Label labelLabel{&labelLayout, Size{50, 0}};
      LineEdit labelEdit{&labelLayout, Size{~0, 0}};
    HorizontalLayout noteLayout{&layout, Size{~0, 0}};
      Label noteLabel{&noteLayout, Size{50, 0}};
      LineEdit noteEdit{&noteLayout, Size{~0, 0}};
  HorizontalLayout lowerLayout{&layout, Size{~0, ~0}};
    Label componentLabel{&lowerLayout, Size{50, ~0}};
    TreeView componentTree{&lowerLayout, Size{~0, ~0}};
    VerticalLayout controlLayout{&lowerLayout, Size{0, ~0}};
      Button appendMemoryButton{&controlLayout, Size{80, 0}};
      Button appendOscillatorButton{&controlLayout, Size{80, 0}};
      Button modifyComponentButton{&controlLayout, Size{80, 0}};
      Button removeComponentButton{&controlLayout, Size{80, 0}};
      Widget controlSpacer{&controlLayout, Size{0, ~0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};

struct MemoryWindow : Window {
  MemoryWindow();
  auto show(Memory = {}) -> void;
  auto accept() -> void;
  auto cancel() -> void;
  auto updateWindow() -> void;

private:
  bool modified = false;
  bool create = true;
  Memory memory;

  VerticalLayout layout{this};
    HorizontalLayout infoLayout{&layout, Size{~0, 0}};
      Label typeLabel{&infoLayout, Size{80, 0}};
      ComboEdit typeEdit{&infoLayout, Size{~0, 0}};
      Label sizeLabel{&infoLayout, Size{0, 0}};
      LineEdit sizeEdit{&infoLayout, Size{~0, 0}};
    HorizontalLayout contentLayout{&layout, Size{~0, 0}};
      Label contentLabel{&contentLayout, Size{80, 0}};
      ComboEdit contentEdit{&contentLayout, Size{~0, 0}};
    HorizontalLayout manufacturerLayout{&layout, Size{~0, 0}};
      Label manufacturerLabel{&manufacturerLayout, Size{80, 0}};
      LineEdit manufacturerEdit{&manufacturerLayout, Size{~0, 0}};
    HorizontalLayout architectureLayout{&layout, Size{~0, 0}};
      Label architectureLabel{&architectureLayout, Size{80, 0}};
      LineEdit architectureEdit{&architectureLayout, Size{~0, 0}};
    HorizontalLayout identifierLayout{&layout, Size{~0, 0}};
      Label identifierLabel{&identifierLayout, Size{80, 0}};
      LineEdit identifierEdit{&identifierLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      CheckLabel batteryOption{&controlLayout, Size{0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};

struct OscillatorWindow : Window {
  OscillatorWindow();
  auto show(Oscillator = {}) -> void;
  auto accept() -> void;
  auto cancel() -> void;
  auto updateWindow() -> void;

private:
  bool modified = false;
  bool create = true;
  Oscillator oscillator;

  VerticalLayout layout{this};
    HorizontalLayout frequencyLayout{&layout, Size{~0, 0}};
      Label frequencyLabel{&frequencyLayout, Size{60, 0}};
      LineEdit frequencyEdit{&frequencyLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};
