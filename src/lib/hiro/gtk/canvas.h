class pCanvas : public pFormControl {
public:
  void create(uint style, uint width, uint height);
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
  uint bpitch;
  void resize(uint width, uint height);
  GtkWidget* gtk_handle();
};
