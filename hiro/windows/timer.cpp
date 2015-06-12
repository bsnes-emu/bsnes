#if defined(Hiro_Timer)

namespace hiro {

static vector<pTimer*> timers;

static auto CALLBACK Timer_timeoutProc(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD time) -> void {
  for(auto& timer : timers) {
    if(timer->htimer == timerID) return timer->self().doActivate();
  }
}

auto pTimer::construct() -> void {
  timers.append(this);
  htimer = 0;
}

auto pTimer::destruct() -> void {
}

auto pTimer::setEnabled(bool enabled) -> void {
  if(htimer) {
    KillTimer(NULL, htimer);
    htimer = 0;
  }

  if(enabled == true) {
    htimer = SetTimer(NULL, 0u, state().interval, Timer_timeoutProc);
  }
}

auto pTimer::setInterval(unsigned interval) -> void {
  //destroy and recreate timer if interval changed
  setEnabled(self().enabled(true));
}

}

#endif
