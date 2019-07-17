#if defined(Hiro_Timer)

namespace hiro {

static auto Timer_trigger(pTimer* p) -> signed {
  //prevent all timers from firing once the program has been terminated
  if(Application::state().quit) return false;

  //timer may have been disabled prior to triggering, so check state
  if(p->self().enabled(true)) p->self().doActivate();

  //callback may have disabled timer, so check state again
  if(p->self().enabled(true)) {
    g_timeout_add(p->state().interval, (GSourceFunc)Timer_trigger, (gpointer)p);
  }

  //kill this timer instance (it is spawned above if needed again)
  return false;
}

auto pTimer::construct() -> void {
}

auto pTimer::destruct() -> void {
}

auto pTimer::setEnabled(bool enabled) -> void {
  if(enabled) {
    g_timeout_add(state().interval, (GSourceFunc)Timer_trigger, (gpointer)this);
  }
}

auto pTimer::setInterval(uint interval) -> void {
}

}

#endif
