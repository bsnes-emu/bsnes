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

  profileResearch = new QRadioButton("Research");
  profileResearch->setStyleSheet("font-weight: bold; font-size: 12pt;");
  layout->addWidget(profileResearch);

  profileResearchInfo = new QLabel(
    "<b>System Requirements:</b> A super-computer cooled by LN<sub>2</sub>.<br>"
    "Maximum accuracy, no matter the cost.<br>"
    "Use this mode for development purposes only."
  );
  profileResearchInfo->setStyleSheet("margin-left: 22px;");
  layout->addWidget(profileResearchInfo);
  layout->addSpacing(Style::WidgetSpacing);

  profileBaseline = new QRadioButton("Baseline (recommended)");
  profileBaseline->setStyleSheet("font-weight: bold; font-size: 12pt;");
  layout->addWidget(profileBaseline);

  profileBaselineInfo = new QLabel(
    "<b>System Requirements:</b> Intel Core Solo or AMD Athlon 64 processor.<br>"
    "Extreme accuracy with reasonable hardware requirements in mind.<br>"
    "Very rarely, slight graphical glitches may appear."
  );
  profileBaselineInfo->setStyleSheet("margin-left: 22px;");
  layout->addWidget(profileBaselineInfo);
  layout->addSpacing(Style::WidgetSpacing);

  profilePerformance = new QRadioButton("Performance");
  profilePerformance->setStyleSheet("font-weight: bold; font-size: 12pt;");
  layout->addWidget(profilePerformance);

  profilePerformanceInfo = new QLabel(
    "<b>System Requirements:</b> Intel Atom, Intel Pentium IV or AMD Athlon processor.<br>"
    "High accuracy with reasonable sacrifices for performance.<br>"
    "Sacrifices a small degree of compatibility to run full-speed on older hardware.<br>"
    "Use this mode only if baseline is too slow, or if you are running on battery power."
  );
  profilePerformanceInfo->setStyleSheet("margin-left: 22px;");
  layout->addWidget(profilePerformanceInfo);

  if(config().system.profile == "research") {
    profileResearch->setChecked(true);
  } else if(config().system.profile == "baseline") {
    profileBaseline->setChecked(true);
  } else if(config().system.profile == "performance") {
    profilePerformance->setChecked(true);
  } else {
    config().system.profile = "baseline";
    profileBaseline->setChecked(true);
    QMessageBox::information(0, "First-Run Notice",
      "<b>Note:</b> bsnes contains multiple emulation profiles.<br><br>"
      "If bsnes runs too slowly, you can double the speed by using the "
      "'Performance' profile; or if you want even more accuracy, you can use the "
      "'Research' profile.<br><br>"
      "Feel free to experiment. You can select different profiles via:<br>"
      "Settings -> Configuration -> Profile"
    );
  }

  connect(profileResearch, SIGNAL(pressed()), this, SLOT(setResearchProfile()));
  connect(profileBaseline, SIGNAL(pressed()), this, SLOT(setBaselineProfile()));
  connect(profilePerformance, SIGNAL(pressed()), this, SLOT(setPerformanceProfile()));
}

void ProfileSettingsWindow::setResearchProfile() {
  config().system.profile = "research";
}

void ProfileSettingsWindow::setBaselineProfile() {
  config().system.profile = "baseline";
}

void ProfileSettingsWindow::setPerformanceProfile() {
  config().system.profile = "performance";
}
