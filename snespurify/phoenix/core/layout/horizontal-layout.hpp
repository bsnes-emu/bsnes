struct VerticalLayout;

struct HorizontalLayout : public Layout {
  void append(VerticalLayout &layout, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  Geometry minimumLayoutGeometry();
  Geometry minimumGeometry();
  void setGeometry(const Geometry &geometry);
  void setMargin(unsigned margin);
  void setParent(Window &parent);
  void setVisible(bool visible);
  HorizontalLayout();

//private:
  unsigned margin;
  struct Children {
    VerticalLayout *layout;
    Widget *widget;
    unsigned width, height, spacing;
  };
  nall::linear_vector<Children> children;
};
