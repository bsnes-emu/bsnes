class pProgressbar : public pFormControl {
public:
  Progressbar &self;
  void create(unsigned style, unsigned width, unsigned height);
  unsigned get_progress();
  void set_progress(unsigned progress);

  pProgressbar(Progressbar&);

  /* internal */
  GtkWidget *progressbar;
  GtkWidget* gtk_handle();
};
