struct FixedLayout : Layout {
  void append(Sizable &sizable, const Geometry &geometry);
  Geometry minimumGeometry();
  void setGeometry(const Geometry &geometry);
  void setLayout(Layout &layout);
  void setParent(Window &parent);
  void setVisible(bool visible);
  bool visible();
  FixedLayout();

//private:
  Layout *layout;
  Window *parent;
  bool visible_;
  struct Children {
    Sizable *sizable;
    Geometry geometry;
  };
  nall::linear_vector<Children> children;
};
