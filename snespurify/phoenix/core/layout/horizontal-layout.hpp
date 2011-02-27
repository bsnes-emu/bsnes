struct VerticalLayout;

struct HorizontalLayout : public Layout {
  void append(VerticalLayout &layout, unsigned width, unsigned height, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  unsigned minimumWidth();
  void setGeometry(Geometry &geometry);
  void setMargin(unsigned margin);
  void setParent(Window &parent);
  void setVisible(bool visible);
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
