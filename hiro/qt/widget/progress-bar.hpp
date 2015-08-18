#if defined(Hiro_ProgressBar)

namespace hiro {

struct pProgressBar : pWidget {
  Declare(ProgressBar, Widget)

  auto minimumSize() const -> Size override;
  auto setPosition(unsigned position) -> void;

  auto _setState() -> void;

  QProgressBar* qtProgressBar = nullptr;
};

}

#endif
