static linear_vector<pTimer*> timers;

static void CALLBACK Timer_timeoutProc(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD time) {
  for(auto &timer : timers) {
    if(timer->htimer == timerID) {
      if(timer->timer.onTimeout) timer->timer.onTimeout();
      return;
    }
  }
}

void pTimer::setEnabled(bool enabled) {
  if(htimer) {
    KillTimer(NULL, htimer);
    htimer = 0;
  }

  if(enabled == true) {
    htimer = SetTimer(NULL, 0U, timer.state.milliseconds, Timer_timeoutProc);
  }
}

void pTimer::setInterval(unsigned milliseconds) {
  //destroy and recreate timer if interval changed
  setEnabled(timer.state.enabled);
}

void pTimer::constructor() {
  timers.append(this);
  htimer = 0;
}
