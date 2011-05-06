struct HorizontalLayout;

struct VerticalLayout : public Layout {
  void append(HorizontalLayout &layout, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  Geometry minimumGeometry();
  Geometry minimumLayoutGeometry();
  void reset();
  void setGeometry(const Geometry &geometry);
  void setMargin(unsigned margin);
  void setParent(Window &parent);
  void setVisible(bool visible);
  VerticalLayout();

//private:
  unsigned margin;
  struct Children {
    HorizontalLayout *layout;
    Widget *widget;
    unsigned width, height, spacing;
  };
  nall::linear_vector<Children> children;
};
