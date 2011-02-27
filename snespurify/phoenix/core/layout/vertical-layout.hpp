struct HorizontalLayout;

struct VerticalLayout : public Layout {
  void append(HorizontalLayout &layout, unsigned width, unsigned height, unsigned spacing = 0);
  void append(Widget &widget, unsigned width, unsigned height, unsigned spacing = 0);
  unsigned minimumHeight();
  void setGeometry(Geometry &geometry);
  void setMargin(unsigned margin);
  void setParent(Window &parent);
  void setVisible(bool visible);
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
