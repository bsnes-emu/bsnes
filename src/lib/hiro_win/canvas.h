class pCanvas : public pFormControl {
public:
  void create(uint style, uint width, uint height);
  void redraw();
  uint32_t* buffer();

  Canvas &self;
  pCanvas(Canvas&);
  ~pCanvas();

  /* internal */
  BITMAPINFO bmi;
  uint32_t *ibuffer;
  uint ipitch, iwidth, iheight;
  void blit();
  void resize(uint width, uint height);
};
