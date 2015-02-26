namespace hiro {

struct pProgressBar : pWidget {
  Declare(ProgressBar, Widget)

  auto minimumSize() const -> Size;
  auto setPosition(unsigned position) -> void;
};

}
