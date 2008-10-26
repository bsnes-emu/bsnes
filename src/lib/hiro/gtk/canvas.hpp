class pCanvas : public pFormControl {
public:
  void create(unsigned style, unsigned width, unsigned height);
  void redraw();
  uint32_t* buffer();

  Canvas &self;
  pCanvas(Canvas&);
  ~pCanvas();

  /* internal */
  GtkWidget *canvas;
  //GTK+ RGB drawing function draws in xBGR format, so two buffers are needed ...
  uint32_t *fbuffer; //one for the xRGB image
  uint32_t *rbuffer; //one for the xBGR image
  unsigned bpitch;
  void resize(unsigned width, unsigned height);
  GtkWidget* gtk_handle();
};
