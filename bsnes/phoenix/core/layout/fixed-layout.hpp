struct FixedLayout : Layout {
  void append(Widget &widget, const Geometry &geometry);
  void setGeometry(const Geometry &geometry);
  void setParent(Window &parent);
  void setVisible(bool visible);
  FixedLayout();

//private:
  Window *parent;
  struct Children {
    Widget *widget;
    Geometry geometry;
  };
  nall::linear_vector<Children> children;
};
