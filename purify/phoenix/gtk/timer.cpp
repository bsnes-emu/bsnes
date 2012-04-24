static guint Timer_trigger(pTimer *self) {
  //timer may have been disabled prior to triggering, so check state
  if(self->timer.state.enabled) {
    if(self->timer.onTimeout) self->timer.onTimeout();
  }
  //callback may have disabled timer, so check state again
  if(self->timer.state.enabled) {
    g_timeout_add(self->timer.state.milliseconds, (GSourceFunc)Timer_trigger, (gpointer)self);
  }
  //kill this timer instance (it is spawned above if needed again)
  return false;
}

void pTimer::setEnabled(bool enabled) {
  if(enabled) {
    g_timeout_add(timer.state.milliseconds, (GSourceFunc)Timer_trigger, (gpointer)this);
  }
}

void pTimer::setInterval(unsigned milliseconds) {
}

void pTimer::constructor() {
}
