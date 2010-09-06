#include "../ui-base.hpp"

#include "profile.cpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "paths.cpp"
#include "advanced.cpp"

#include "settings.moc"
SettingsWindow *settingsWindow;

SettingsWindow::SettingsWindow() {
  setObjectName("settings-window");
  setWindowTitle("Configuration Settings");
  resize(600, 360);
  setGeometryString(&config().geometry.settingsWindow);
  application.windowList.append(this);

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  profileSettingsWindow  = new ProfileSettingsWindow;
  videoSettingsWindow    = new VideoSettingsWindow;
  audioSettingsWindow    = new AudioSettingsWindow;
  inputSettingsWindow    = new InputSettingsWindow;
  pathSettingsWindow     = new PathSettingsWindow;
  advancedSettingsWindow = new AdvancedSettingsWindow;

  profileArea = new QScrollArea;
  profileArea->setWidget(profileSettingsWindow);
  profileArea->setFrameStyle(0);
  profileArea->setWidgetResizable(true);

  videoArea = new QScrollArea;
  videoArea->setWidget(videoSettingsWindow);
  videoArea->setFrameStyle(0);
  videoArea->setWidgetResizable(true);

  audioArea = new QScrollArea;
  audioArea->setWidget(audioSettingsWindow);
  audioArea->setFrameStyle(0);
  audioArea->setWidgetResizable(true);

  inputArea = new QScrollArea;
  inputArea->setWidget(inputSettingsWindow);
  inputArea->setFrameStyle(0);
  inputArea->setWidgetResizable(true);

  pathArea = new QScrollArea;
  pathArea->setWidget(pathSettingsWindow);
  pathArea->setFrameStyle(0);
  pathArea->setWidgetResizable(true);

  advancedArea = new QScrollArea;
  advancedArea->setWidget(advancedSettingsWindow);
  advancedArea->setFrameStyle(0);
  advancedArea->setWidgetResizable(true);

  tab = new QTabWidget;
  tab->addTab(profileArea, "Profile");
  tab->addTab(videoArea, "Video");
  tab->addTab(audioArea, "Audio");
  tab->addTab(inputArea, "Input");
  tab->addTab(pathArea, "Paths");
  tab->addTab(advancedArea, "Advanced");
  layout->addWidget(tab);
}
