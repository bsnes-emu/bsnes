struct VerticalLayout;

struct HorizontalLayout : public Layout {
  void setParent(Window &parent);
  void setGeometry(Geometry &geometry);
  void append(VerticalLayout &layout, unsigned width, unsigned height, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  void setMargin(unsigned margin);
  unsigned minimumWidth();
  HorizontalLayout();

//private:
  unsigned margin;
  unsigned width;
  unsigned height;
  struct Children {
    VerticalLayout *layout;
    Widget *widget;
    unsigned width, height, spacing;
  };
  nall::linear_vector<Children> children;
};
