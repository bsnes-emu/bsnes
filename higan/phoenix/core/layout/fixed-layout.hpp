struct FixedLayout : Layout {
  void append(Sizable& sizable, const Geometry& geometry);
  void append(Sizable& sizable);
  bool enabled();
  Size minimumSize();
  void remove(Sizable& sizable);
  void reset();
  void setEnabled(bool enabled = true);
  void setGeometry(const Geometry& geometry);
  void setVisible(bool visible = true);
  void synchronizeLayout();
  bool visible();
  FixedLayout();
  ~FixedLayout();

//private:
  struct State {
    bool enabled;
    bool visible;
  } state;

  struct Children {
    Sizable* sizable;
    Geometry geometry;
  };
  nall::vector<Children> children;
};
