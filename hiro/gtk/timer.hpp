#if defined(Hiro_Timer)

namespace hiro {

struct pTimer : pObject {
  Declare(Timer, Object)

  auto setEnabled(bool enabled) -> void;
  auto setInterval(unsigned interval) -> void;
};

}

#endif
