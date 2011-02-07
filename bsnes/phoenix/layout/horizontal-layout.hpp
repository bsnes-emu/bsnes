struct VerticalLayout;

struct HorizontalLayout : public Layout {
  void setParent(Window &parent);
  void append(VerticalLayout &layout, unsigned width, unsigned height, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  void update(Geometry &geometry);
  void setMargin(unsigned margin);
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
