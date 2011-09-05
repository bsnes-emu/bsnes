struct FixedLayout : Layout {
  void append(Sizable &sizable, const Geometry &geometry);
  void append(Sizable &sizable);
  bool enabled();
  Geometry minimumGeometry();
  void remove(Sizable &sizable);
  void reset();
  void setEnabled(bool enabled = true);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible = true);
  void synchronize();
  bool visible();
  FixedLayout();
  ~FixedLayout();

//private:
  struct State {
    bool enabled;
    bool visible;
  } state;

  struct Children {
    Sizable *sizable;
    Geometry geometry;
  };
  nall::linear_vector<Children> children;
};
