class pCanvas : public pFormControl {
public:
  void create(uint style, uint width, uint height);
  void redraw();
  uint32* buffer();

  Canvas &self;
  pCanvas(Canvas&);
  ~pCanvas();

/* internal */
  BITMAPINFO bmi;
  uint32 *ibuffer;
  uint ipitch, iwidth, iheight;
  void blit();
  void resize(uint width, uint height);
};
