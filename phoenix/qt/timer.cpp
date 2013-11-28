namespace phoenix {

void pTimer::setEnabled(bool enabled) {
  if(enabled) {
    qtTimer->start();
  } else {
    qtTimer->stop();
  }
}

void pTimer::setInterval(unsigned interval) {
  qtTimer->setInterval(interval);
}

void pTimer::constructor() {
  qtTimer = new QTimer;
  qtTimer->setInterval(0);
  connect(qtTimer, SIGNAL(timeout()), SLOT(onActivate()));
}

void pTimer::destructor() {
  delete qtTimer;
}

void pTimer::onActivate() {
  if(timer.onActivate) timer.onActivate();
}

}
