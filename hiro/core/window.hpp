#if defined(Hiro_Window)
struct mWindow : mObject {
  Declare(Window)
  using mObject::remove;

  mWindow();

  auto append(sMenuBar menuBar) -> type&;
  auto append(sSizable sizable) -> type&;
  auto append(sStatusBar statusBar) -> type&;
  auto backgroundColor() const -> Color;
  auto dismissable() const -> bool;
  auto doClose() const -> void;
  auto doDrop(vector<string>) const -> void;
  auto doKeyPress(int) const -> void;
  auto doKeyRelease(int) const -> void;
  auto doMove() const -> void;
  auto doSize() const -> void;
  auto droppable() const -> bool;
  auto frameGeometry() const -> Geometry;
  auto fullScreen() const -> bool;
  auto geometry() const -> Geometry;
  auto handle() const -> uintptr_t;
  auto maximized() const -> bool;
  auto maximumSize() const -> Size;
  auto menuBar() const -> MenuBar;
  auto minimized() const -> bool;
  auto minimumSize() const -> Size;
  auto modal() const -> bool;
  auto monitor() const -> uint;
  auto onClose(const function<void ()>& callback = {}) -> type&;
  auto onDrop(const function<void (vector<string>)>& callback = {}) -> type&;
  auto onKeyPress(const function<void (int)>& callback = {}) -> type&;
  auto onKeyRelease(const function<void (int)>& callback = {}) -> type&;
  auto onMove(const function<void ()>& callback = {}) -> type&;
  auto onSize(const function<void ()>& callback = {}) -> type&;
  auto remove(sMenuBar menuBar) -> type&;
  auto remove(sSizable sizable) -> type&;
  auto remove(sStatusBar statusBar) -> type&;
  auto reset() -> type& override;
  auto resizable() const -> bool;
  auto setAlignment(Alignment = Alignment::Center) -> type&;
  auto setAlignment(sWindow relativeTo, Alignment = Alignment::Center) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setDismissable(bool dismissable = true) -> type&;
  auto setDroppable(bool droppable = true) -> type&;
  auto setFrameGeometry(Geometry geometry) -> type&;
  auto setFramePosition(Position position) -> type&;
  auto setFrameSize(Size size) -> type&;
  auto setFullScreen(bool fullScreen = true) -> type&;
  auto setGeometry(Geometry geometry) -> type&;
  auto setMaximized(bool maximized = true) -> type&;
  auto setMaximumSize(Size size = {}) -> type&;
  auto setMinimized(bool minimized = true) -> type&;
  auto setMinimumSize(Size size = {}) -> type&;
  auto setModal(bool modal = true) -> type&;
  auto setPosition(Position) -> type&;
  auto setPosition(sWindow relativeTo, Position) -> type&;
  auto setResizable(bool resizable = true) -> type&;
  auto setSize(Size size) -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto setVisible(bool visible = true) -> type&;
  auto sizable() const -> Sizable;
  auto statusBar() const -> StatusBar;
  auto title() const -> string;

//private:
  struct State {
    Color backgroundColor;
    bool dismissable = false;
    bool droppable = false;
    bool fullScreen = false;
    Geometry geometry = {128, 128, 256, 256};
    bool maximized = false;
    Size maximumSize;
    bool minimized = false;
    Size minimumSize;
    sMenuBar menuBar;
    bool modal = false;
    function<void ()> onClose;
    function<void (vector<string>)> onDrop;
    function<void (int)> onKeyPress;
    function<void (int)> onKeyRelease;
    function<void ()> onMove;
    function<void ()> onSize;
    bool resizable = true;
    sSizable sizable;
    sStatusBar statusBar;
    string title;
  } state;

  auto destruct() -> void;
};
#endif
