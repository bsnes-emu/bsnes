#if defined(Hiro_FixedLayout)

struct mFixedLayout : mLayout {
  using type = mFixedLayout;
  using mLayout::append;
  using mLayout::remove;

  auto append(sSizable sizable, Geometry geometry) -> type&;
  auto minimumSize() const -> Size override;
  auto remove(sSizable sizable) -> type& override;
  auto reset() -> type& override;
  auto setEnabled(bool enabled = true) -> type& override;
  auto setFont(const string& font = "") -> type& override;
  auto setVisible(bool visible = true) ->type& override;

  struct Properties {
    Geometry geometry;
  };
  vector<Properties> properties;
};

#endif
