struct FixedLayout : Layout {
  void setParent(Window &parent);
  void setGeometry(Geometry &geometry);
  void append(Widget &widget, const Geometry &geometry);
  FixedLayout();

//private:
  Window *parent;
  struct Children {
    Widget *widget;
    Geometry geometry;
  };
  nall::linear_vector<Children> children;
};
