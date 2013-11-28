namespace phoenix {

struct pCanvas : public pWidget {
  Canvas& canvas;

  void setDroppable(bool droppable);
  void setMode(Canvas::Mode mode);
  void setSize(Size size);

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
};

}
