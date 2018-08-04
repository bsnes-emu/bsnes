#if defined(Hiro_TableView)
struct mTableViewColumn : mObject {
  Declare(TableViewColumn)

  auto active() const -> bool;
  auto alignment() const -> Alignment;
  auto backgroundColor() const -> Color;
  auto editable() const -> bool;
  auto expandable() const -> bool;
  auto foregroundColor() const -> Color;
  auto horizontalAlignment() const -> float;
  auto icon() const -> image;
  auto remove() -> type& override;
  auto resizable() const -> bool;
  auto setActive() -> type&;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setExpandable(bool expandable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setHorizontalAlignment(float alignment = 0.0) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setResizable(bool resizable = true) -> type&;
  auto setSorting(Sort sorting = Sort::None) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setVerticalAlignment(float alignment = 0.5) -> type&;
  auto setVisible(bool visible = true) -> type&;
  auto setWidth(float width = 0) -> type&;
  auto sort(Sort sorting) -> type&;
  auto sorting() const -> Sort;
  auto text() const -> string;
  auto verticalAlignment() const -> float;
  auto width() const -> float;

//private:
  struct State {
    bool active = false;
    Alignment alignment;
    Color backgroundColor;
    bool editable = false;
    bool expandable = false;
    Color foregroundColor;
    float horizontalAlignment = 0.0;
    image icon;
    bool resizable = true;
    Sort sorting = Sort::None;
    string text;
    float verticalAlignment = 0.5;
    bool visible = true;
    float width = 0;
  } state;
};
#endif
