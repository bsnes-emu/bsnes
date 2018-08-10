#if defined(Hiro_Timer)

//timeBeginPeriod(1) + timeSetEvent does not seem any more performant than SetTimer
//it also seems buggier, and requires libwinmm

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
  setEnabled(false);
  if(auto index = timers.find(this)) timers.remove(*index);
}

auto pTimer::setEnabled(bool enabled) -> void {
  if(htimer) {
    KillTimer(nullptr, htimer);
    htimer = 0;
  }

  if(enabled == true) {
    htimer = SetTimer(nullptr, 0, state().interval, Timer_timeoutProc);
  }
}

auto pTimer::setInterval(uint interval) -> void {
  //destroy and recreate timer if interval changed
  setEnabled(self().enabled(true));
}

}

#endif
