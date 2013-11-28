struct FixedLayout : Layout {
  void append(Sizable& sizable, Geometry geometry);
  void append(Sizable& sizable);
  Size minimumSize();
  void remove(Sizable& sizable);
  void reset();
  void setEnabled(bool enabled = true);
  void setGeometry(Geometry geometry);
  void setVisible(bool visible = true);
  void synchronizeLayout();
  ~FixedLayout();

//private:
  struct Children {
    Sizable* sizable;
    Geometry geometry;
  };
  nall::vector<Children> children;
};
