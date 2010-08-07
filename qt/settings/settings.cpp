#include "../ui-base.hpp"

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

  videoSettingsWindow    = new VideoSettingsWindow;
  audioSettingsWindow    = new AudioSettingsWindow;
  inputSettingsWindow    = new InputSettingsWindow;
  pathSettingsWindow     = new PathSettingsWindow;
  advancedSettingsWindow = new AdvancedSettingsWindow;

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
  tab->addTab(videoArea, QIcon(":/16x16/video-display.png"), "Video");
  tab->addTab(audioArea, QIcon(":/16x16/audio-volume-high.png"), "Audio");
  tab->addTab(inputArea, QIcon(":/16x16/input-gaming.png"), "Input");
  tab->addTab(pathArea, QIcon(":/16x16/folder.png"), "Paths");
  tab->addTab(advancedArea, QIcon(":/16x16/preferences-system.png"), "Advanced");
  layout->addWidget(tab);
}
