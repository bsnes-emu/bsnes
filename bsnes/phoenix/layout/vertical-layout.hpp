struct HorizontalLayout;

struct VerticalLayout : public Layout {
  void setParent(Window &parent);
  void append(HorizontalLayout &layout, unsigned width, unsigned height, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  void update(Geometry &geometry);
  void setMargin(unsigned margin);
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
