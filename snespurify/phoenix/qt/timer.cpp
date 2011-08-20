void pTimer::setEnabled(bool enabled) {
  if(enabled) {
    qtTimer->start();
  } else {
    qtTimer->stop();
  }
}

void pTimer::setInterval(unsigned milliseconds) {
  qtTimer->setInterval(milliseconds);
}

void pTimer::constructor() {
  qtTimer = new QTimer;
  qtTimer->setInterval(0);
  connect(qtTimer, SIGNAL(timeout()), SLOT(onTimeout()));
}

void pTimer::onTimeout() {
  if(timer.onTimeout) timer.onTimeout();
}
