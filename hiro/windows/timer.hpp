#if defined(Hiro_Timer)

namespace hiro {

struct pTimer : pObject {
  Declare(Timer, Object)

  auto setEnabled(bool enabled) -> void override;
  auto setInterval(unsigned interval) -> void;

  UINT_PTR htimer;
};

}

#endif
