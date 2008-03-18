class pProgressbar : public pFormControl {
public:
  Progressbar &self;
  void create(uint style, uint width, uint height);
  uint get_progress();
  void set_progress(uint progress);

  pProgressbar(Progressbar&);

  /* internal */
  GtkWidget *progressbar;
  GtkWidget* gtk_handle();
};
