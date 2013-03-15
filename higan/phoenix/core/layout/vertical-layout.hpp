struct VerticalLayout : public Layout {
  void append(Sizable &sizable, const Size &size, unsigned spacing = 0);
  void append(Sizable &sizable);
  bool enabled();
  Size minimumSize();
  void remove(Sizable &sizable);
  void reset();
  void setAlignment(double alignment);
  void setEnabled(bool enabled = true);
  void setGeometry(const Geometry &geometry);
  void setMargin(unsigned margin);
  void setVisible(bool visible = true);
  void synchronizeLayout();
  bool visible();
  VerticalLayout();
  ~VerticalLayout();

//private:
  struct State {
    double alignment;
    bool enabled;
    unsigned margin;
    bool visible;
  } state;

  struct Children {
    Sizable *sizable;
    unsigned width, height, spacing;
  };
  nall::vector<Children> children;
};
