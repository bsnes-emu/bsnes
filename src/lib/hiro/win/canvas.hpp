class pCanvas : public pFormControl {
public:
  void create(unsigned style, unsigned width, unsigned height);
  void redraw();
  uint32_t* buffer();

  Canvas &self;
  pCanvas(Canvas&);
  ~pCanvas();

  /* internal */
  BITMAPINFO bmi;
  uint32_t *ibuffer;
  unsigned ipitch, iwidth, iheight;
  void resize(unsigned width, unsigned height);
};
