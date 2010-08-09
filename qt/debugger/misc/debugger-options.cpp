#include "debugger-options.moc"
DebuggerOptions *debuggerOptions;

DebuggerOptions::DebuggerOptions() {
  setObjectName("debugger-options");
  setWindowTitle("Debugger Options");
  setGeometryString(&config().geometry.debuggerOptions);
  application.windowList.append(this);

  layout = new QVBoxLayout;
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  cacheUsageBox = new QCheckBox("Cache memory usage table to disk");
  layout->addWidget(cacheUsageBox);

  synchronize();
  connect(cacheUsageBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCacheUsage()));
}

void DebuggerOptions::synchronize() {
  cacheUsageBox->setChecked(config().debugger.cacheUsageToDisk);
}

void DebuggerOptions::toggleCacheUsage() {
  config().debugger.cacheUsageToDisk = cacheUsageBox->isChecked();
}
