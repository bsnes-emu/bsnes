#if defined(Hiro_HorizontalLayout)

struct mHorizontalLayout : mLayout {
  using type = mHorizontalLayout;
  using mLayout::append;
  using mLayout::remove;

  auto append(sSizable sizable, Size size, signed spacing = 5) -> type&;
  auto minimumSize() const -> Size override;
  auto remove(sSizable sizable) -> type& override;
  auto reset() -> type& override;
  auto setAlignment(double alignment = 0.5) -> type&;
  auto setEnabled(bool enabled = true) -> type& override;
  auto setFont(const string& font = "") -> type& override;
  auto setGeometry(Geometry geometry) -> type& override;
  auto setMargin(signed margin = 0) -> type&;
  auto setSpacing(signed spacing = 5) -> type&;
  auto setVisible(bool visible = true) -> type&;

  struct Settings {
    double alignment = 0.5;
    signed margin = 0;
    signed spacing = 5;
  } settings;

  struct Property {
    signed width;
    signed height;
    signed spacing;
  };
  vector<Property> properties;
};

#endif
