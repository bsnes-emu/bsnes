struct HorizontalLayout : public Layout {
  void append(Sizable& sizable, Size size, unsigned spacing = 0);
  void append(Sizable& sizable);
  Size minimumSize();
  void remove(Sizable& sizable);
  void reset();
  void setAlignment(double alignment);
  void setEnabled(bool enabled = true);
  void setGeometry(Geometry geometry);
  void setMargin(unsigned margin);
  void setVisible(bool visible = true);
  void synchronizeLayout();
  ~HorizontalLayout();

//private:
  struct State {
    double alignment = 0.5;
    unsigned margin = 0;
  } state;

  struct Children {
    Sizable* sizable;
    unsigned width, height, spacing;
  };
  nall::vector<Children> children;
};
