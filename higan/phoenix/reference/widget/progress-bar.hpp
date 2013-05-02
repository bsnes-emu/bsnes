namespace phoenix {

struct pProgressBar : public pWidget {
  ProgressBar& progressBar;

  void setPosition(unsigned position);

  pProgressBar(ProgressBar& progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void destructor();
};

}
