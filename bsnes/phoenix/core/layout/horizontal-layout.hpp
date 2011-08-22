struct VerticalLayout;

struct HorizontalLayout : public Layout {
  void append(Sizable &sizable, unsigned width, unsigned height, unsigned spacing = 0);
  Geometry minimumGeometry();
  Geometry minimumLayoutGeometry();
  void setAlignment(double alignment);
  void setGeometry(const Geometry &geometry);
  void setLayout(Layout &layout);
  void setMargin(unsigned margin);
  void setParent(Window &parent);
  void setVisible(bool visible);
  bool visible();
  HorizontalLayout();

//private:
  double alignment;
  Layout *layout;
  unsigned margin;
  bool visible_;
  struct Children {
    Sizable *sizable;
    unsigned width, height, spacing;
  };
  nall::linear_vector<Children> children;
};
