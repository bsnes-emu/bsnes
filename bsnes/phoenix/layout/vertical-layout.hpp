struct HorizontalLayout;

struct VerticalLayout : public Layout {
  void setParent(Window &parent);
  void setGeometry(Geometry &geometry);
  void append(HorizontalLayout &layout, unsigned width, unsigned height, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  void setMargin(unsigned margin);
  unsigned minimumHeight();
  VerticalLayout();

//private:
  unsigned margin;
  unsigned width;
  unsigned height;
  struct Children {
    HorizontalLayout *layout;
    Widget *widget;
    unsigned width, height, spacing;
  };
  nall::linear_vector<Children> children;
};
