#if defined(Hiro_ProgressBar)

namespace hiro {

struct pProgressBar : pWidget {
  Declare(ProgressBar, Widget)

  auto minimumSize() const -> Size override;
  auto setPosition(unsigned position) -> void;
};

}

#endif
