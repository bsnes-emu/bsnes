#if defined(Hiro_TabFrame)
struct mTabFrameItem : mObject {
  Declare(TabFrameItem)

  auto append(sSizable sizable) -> type&;
  auto closable() const -> bool;
  auto icon() const -> image;
  auto movable() const -> bool;
  auto remove() -> type& override;
  auto remove(sSizable sizable) -> type&;
  auto reset() -> type&;
  auto selected() const -> bool;
  auto setClosable(bool closable = true) -> type&;
  auto setEnabled(bool enabled = true) -> type& override;
  auto setFont(const Font& font = {}) -> type& override;
  auto setIcon(const image& icon = {}) -> type&;
  auto setMovable(bool movable = true) -> type&;
  auto setParent(mObject* object = nullptr, int offset = -1) -> type& override;
  auto setSelected() -> type&;
  auto setText(const string& text = "") -> type&;
  auto setVisible(bool visible = true) -> type& override;
  auto sizable() const -> Sizable;
  auto text() const -> string;

//private:
  struct State {
    bool closable = false;
    image icon;
    bool movable = false;
    bool selected = false;
    sSizable sizable;
    string text;
  } state;

  auto destruct() -> void override;
};
#endif
