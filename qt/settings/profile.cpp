#include "profile.moc"
ProfileSettingsWindow *profileSettingsWindow;

ProfileSettingsWindow::ProfileSettingsWindow() {
  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0);
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  profileInfo = new QLabel(
    "Profiles allow you to balance emulation accuracy against system performance.<br>"
    "Note that you must restart bsnes for profile changes to take effect!<br>"
    "Also, while save RAM is compatible between profiles, save states are not cross-compatible."
  );
  layout->addWidget(profileInfo);
  layout->addSpacing(Style::WidgetSpacing);

  profileAccuracy = new QRadioButton("Accuracy");
  profileAccuracy->setStyleSheet(
    "font-weight: bold;"
    "font-size: 12pt;"
    "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 rgba(255, 0, 0, 48), stop: 1 rgba(255, 0, 0, 0));"
  );
  layout->addWidget(profileAccuracy);

  profileAccuracyInfo = new QLabel(
    "<b>System Requirements:</b> A super-computer cooled by LN<sub>2</sub>.<br>"
    "Maximum accuracy, no matter the cost.<br>"
    "Use this mode for development or research purposes."
  );
  profileAccuracyInfo->setStyleSheet("margin-left: 22px;");
  layout->addWidget(profileAccuracyInfo);
  layout->addSpacing(Style::WidgetSpacing);

  profileCompatibility = new QRadioButton("Compatibility");
  profileCompatibility->setStyleSheet(
    "font-weight: bold;"
    "font-size: 12pt;"
    "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 rgba(0, 0, 255, 48), stop: 1 rgba(0, 0, 255, 0));"
  );
  layout->addWidget(profileCompatibility);

  profileCompatibilityInfo = new QLabel(
    "<b>System Requirements:</b> Intel Core Solo or AMD Athlon 64 processor.<br>"
    "Extreme accuracy with reasonable hardware requirements in mind.<br>"
    "Very rarely, slight graphical glitches may appear in a small number of games."
  );
  profileCompatibilityInfo->setStyleSheet("margin-left: 22px;");
  layout->addWidget(profileCompatibilityInfo);
  layout->addSpacing(Style::WidgetSpacing);

  profilePerformance = new QRadioButton("Performance");
  profilePerformance->setStyleSheet(
    "font-weight: bold;"
    "font-size: 12pt;"
    "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 rgba(0, 255, 0, 48), stop: 1 rgba(0, 255, 0, 0));"
  );
  layout->addWidget(profilePerformance);

  profilePerformanceInfo = new QLabel(
    "<b>System Requirements:</b> Intel Atom, Intel Pentium IV or AMD Athlon processor.<br>"
    "High accuracy with reasonable compromises for performance.<br>"
    "Sacrifices a small degree of compatibility to run full-speed on older hardware.<br>"
    "Use this mode for slower systems, or if you are running on battery power."
  );
  profilePerformanceInfo->setStyleSheet("margin-left: 22px;");
  layout->addWidget(profilePerformanceInfo);

  if(config().system.profile == "accuracy") {
    profileAccuracy->setChecked(true);
  } else if(config().system.profile == "compatibility") {
    profileCompatibility->setChecked(true);
  } else if(config().system.profile == "performance") {
    profilePerformance->setChecked(true);
  } else {
    config().system.profile = "compatibility";
    profileCompatibility->setChecked(true);
    QMessageBox::information(0, "First-Run Notice",
      "<b>Note:</b> bsnes contains multiple emulation profiles.<br><br>"
      "If bsnes runs too slowly, you can greatly increase the speed by using the "
      "'Performance' profile; or if you want even more accuracy, you can use the "
      "'Accuracy' profile.<br><br>"
      "Feel free to experiment. You can select different profiles via:<br>"
      "Settings -> Configuration -> Profile"
    );
  }

  connect(profileAccuracy, SIGNAL(pressed()), this, SLOT(setAccuracyProfile()));
  connect(profileCompatibility, SIGNAL(pressed()), this, SLOT(setCompatibilityProfile()));
  connect(profilePerformance, SIGNAL(pressed()), this, SLOT(setPerformanceProfile()));
}

void ProfileSettingsWindow::setAccuracyProfile() {
  config().system.profile = "accuracy";
}

void ProfileSettingsWindow::setCompatibilityProfile() {
  config().system.profile = "compatibility";
}

void ProfileSettingsWindow::setPerformanceProfile() {
  config().system.profile = "performance";
}
