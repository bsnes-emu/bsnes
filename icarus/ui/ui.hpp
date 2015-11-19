struct ScanDialog : Window {
  ScanDialog();

  auto show() -> void;
  auto refresh() -> void;
  auto activate() -> void;
  auto import() -> void;

  auto gamePakType(const string& type) -> bool;
  auto gameRomType(const string& type) -> bool;

  VerticalLayout layout{this};
    HorizontalLayout pathLayout{&layout, Size{~0, 0}};
      LineEdit pathEdit{&pathLayout, Size{~0, 0}, 0};
      Button refreshButton{&pathLayout, Size{0, 0}, 0};
      Button homeButton{&pathLayout, Size{0, 0}, 0};
      Button upButton{&pathLayout, Size{0, 0}, 0};
    ListView scanList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button selectAllButton{&controlLayout, Size{100, 0}};
      Button unselectAllButton{&controlLayout, Size{100, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button settingsButton{&controlLayout, Size{100, 0}};
      Button importButton{&controlLayout, Size{100, 0}};
};

struct SettingsDialog : Window {
  SettingsDialog();

  VerticalLayout layout{this};
    HorizontalLayout locationLayout{&layout, Size{~0, 0}};
      Label locationLabel{&locationLayout, Size{0, 0}};
      LineEdit locationEdit{&locationLayout, Size{~0, 0}};
      Button changeLocationButton{&locationLayout, Size{80, 0}};
    CheckLabel createManifestsOption{&layout, Size{~0, 0}, 2};
    CheckLabel useDatabaseOption{&layout, Size{~0, 0}};
};

struct ImportDialog : Window {
  ImportDialog();
  auto run(lstring locations) -> void;

  bool abort;
  lstring errors;

  VerticalLayout layout{this};
    Label statusLabel{&layout, Size{~0, 0}};
    ProgressBar progressBar{&layout, Size{~0, 0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button stopButton{&controlLayout, Size{80, 0}};
};

struct ErrorDialog : Window {
  ErrorDialog();
  auto show(const string& text) -> void;

  VerticalLayout layout{this};
    TextEdit errorLog{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget spacer{&controlLayout, Size{~0, 0}};
      Button closeButton{&controlLayout, Size{80, 0}};
};

ScanDialog* scanDialog = nullptr;
SettingsDialog* settingsDialog = nullptr;
ImportDialog* importDialog = nullptr;
ErrorDialog* errorDialog = nullptr;
