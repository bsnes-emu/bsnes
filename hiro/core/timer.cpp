#if defined(Hiro_Timer)

auto mTimer::allocate() -> pObject* {
  return new pTimer(*this);
}

//

auto mTimer::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mTimer::interval() const -> unsigned {
  return state.interval;
}

auto mTimer::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mTimer::setInterval(unsigned interval) -> type& {
  state.interval = interval;
  signal(setInterval, interval);
  return *this;
}

#endif
