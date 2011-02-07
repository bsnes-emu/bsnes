struct FixedLayout : public Layout {
  void setParent(Window &parent);
  void append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height);
  void update(Geometry &geometry);
  FixedLayout();

//private:
  Window *parent;
  struct Children {
    Widget *widget;
    unsigned x, y;
    unsigned width, height;
  };
  nall::linear_vector<Children> children;
};
