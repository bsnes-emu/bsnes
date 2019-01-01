#if defined(Hiro_Timer)

namespace hiro {

auto pTimer::construct() -> void {
  qtTimer = new QtTimer(*this);
  qtTimer->setInterval(0);
  qtTimer->connect(qtTimer, SIGNAL(timeout()), SLOT(onActivate()));
}

auto pTimer::destruct() -> void {
  delete qtTimer;
}

auto pTimer::setEnabled(bool enabled) -> void {
  if(enabled) {
    qtTimer->start();
  } else {
    qtTimer->stop();
  }
}

auto pTimer::setInterval(unsigned interval) -> void {
  qtTimer->setInterval(interval);
}

auto QtTimer::onActivate() -> void {
  if(Application::state().quit) return;
  p.self().doActivate();
}

}

#endif
