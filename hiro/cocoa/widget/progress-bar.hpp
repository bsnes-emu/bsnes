#if defined(Hiro_ProgressBar)

@interface CocoaProgressBar : NSProgressIndicator {
@public
  hiro::mProgressBar* progressBar;
}
-(id) initWith:(hiro::mProgressBar&)progressBar;
@end

namespace hiro {

struct pProgressBar : pWidget {
  Declare(ProgressBar, Widget)

  auto minimumSize() const -> Size override;
  auto setPosition(uint position) -> void;

  CocoaProgressBar* cocoaProgressBar = nullptr;
};

}

#endif
