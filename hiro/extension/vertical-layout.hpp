struct mVerticalLayout : mLayout {
  using type = mVerticalLayout;
  using mLayout::append;
  using mLayout::remove;

  auto append(nall::shared_pointer<mSizable> sizable, Size size, signed spacing = 5) -> type&;
  auto minimumSize() const -> Size override;
  auto remove(nall::shared_pointer<mSizable> sizable) -> type& override;
  auto reset() -> type& override;
  auto setAlignment(double alignment = 0.0) -> type&;
  auto setEnabled(bool enabled = true) -> type& override;
  auto setFont(const nall::string& font = "") -> type& override;
  auto setGeometry(Geometry geometry) -> type& override;
  auto setMargin(signed margin = 0) -> type&;
  auto setSpacing(signed spacing = 5) -> type&;
  auto setVisible(bool visible = true) -> type& override;

  struct Settings {
    double alignment = 0.0;
    signed margin = 0;
    signed spacing = 5;
  } settings;

  struct Properties {
    signed width;
    signed height;
    signed spacing;
  };
  nall::vector<Properties> properties;
};
