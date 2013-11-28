namespace phoenix {

struct pFrame : public pWidget {
  Frame& frame;

  void setText(string text);

  pFrame(Frame& frame) : pWidget(frame), frame(frame) {}
  void constructor();
  void destructor();
};

}
