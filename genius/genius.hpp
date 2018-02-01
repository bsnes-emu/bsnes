struct Memory {
  string type;
  string size;
  string name;
};

struct Game {
  string sha256;
  string region;
  string revision;
  string board;
  string name;
  string label;
  string note;
  vector<Memory> memories;
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
  auto appendMemory(Memory) -> void;
  auto modifyMemory(Memory) -> void;
  auto removeMemory() -> void;

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
      LineEdit boardEdit{&infoLayout, Size{~0, 0}};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{50, 0}};
      LineEdit nameEdit{&nameLayout, Size{~0, 0}};
    HorizontalLayout labelLayout{&layout, Size{~0, 0}};
      Label labelLabel{&labelLayout, Size{50, 0}};
      LineEdit labelEdit{&labelLayout, Size{~0, 0}};
    HorizontalLayout noteLayout{&layout, Size{~0, 0}};
      Label noteLabel{&noteLayout, Size{50, 0}};
      LineEdit noteEdit{&noteLayout, Size{~0, 0}};
  TableView memoryList{&layout, Size{~0, ~0}};
  HorizontalLayout controlLayout{&layout, Size{~0, 0}};
    Button appendButton{&controlLayout, Size{80, 0}};
    Button modifyButton{&controlLayout, Size{80, 0}};
    Button removeButton{&controlLayout, Size{80, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
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
      Label typeLabel{&infoLayout, Size{40, 0}};
      ComboEdit typeEdit{&infoLayout, Size{~0, 0}};
      Label sizeLabel{&infoLayout, Size{0, 0}};
      LineEdit sizeEdit{&infoLayout, Size{~0, 0}};
    HorizontalLayout nameLayout{&layout, Size{~0, 0}};
      Label nameLabel{&nameLayout, Size{40, 0}};
      LineEdit nameEdit{&nameLayout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
      Button cancelButton{&controlLayout, Size{80, 0}};
};
