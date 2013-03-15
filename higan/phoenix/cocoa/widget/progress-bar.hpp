@interface CocoaProgressBar : NSProgressIndicator {
@public
  phoenix::ProgressBar *progressBar;
}
-(id) initWith :(phoenix::ProgressBar&)progressBar;
@end

namespace phoenix {

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;
  CocoaProgressBar *cocoaProgressBar;

  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void destructor();
};

}
