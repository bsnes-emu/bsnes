#if defined(Hiro_FixedLayout)

struct mFixedLayout : mLayout {
  using type = mFixedLayout;
  using mLayout::append;
  using mLayout::remove;

  auto append(nall::shared_pointer<mSizable> sizable, Geometry geometry) -> type&;
  auto minimumSize() const -> Size override;
  auto remove(nall::shared_pointer<mSizable> sizable) -> type& override;
  auto reset() -> type& override;
  auto setEnabled(bool enabled = true) -> type& override;
  auto setFont(const nall::string& font = "") -> type& override;
  auto setVisible(bool visible = true) ->type& override;

  struct Properties {
    Geometry geometry;
  };
  nall::vector<Properties> properties;
};

#endif
