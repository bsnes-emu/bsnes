#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "paths.cpp"
#include "cheateditor.cpp"
#include "advanced.cpp"

#include "utility/inputcapture.cpp"
#include "utility/codeeditor.cpp"

void SettingsWindow::setup() {
  window = new QWidget;
  window->setObjectName("settings-window");
  window->setWindowTitle("Configuration Settings");

  list = new QListWidget;
    video      = new QListWidgetItem("Video");
    audio      = new QListWidgetItem("Audio");
    input      = new QListWidgetItem("Input");
    paths      = new QListWidgetItem("Paths");
    cheatcodes = new QListWidgetItem("Cheat Codes");
    advanced   = new QListWidgetItem("Advanced");
  list->addItem(video);
  list->addItem(audio);
  list->addItem(input);
  list->addItem(paths);
  list->addItem(cheatcodes);
  list->addItem(advanced);
  list->setCurrentItem(input);  //select most frequently used panel by default
  list->setFixedWidth(135);
  list->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  panel = new QWidget;
  panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  layout->addWidget(list);
  layout->addWidget(panel);
  window->setLayout(layout);

  winVideoSettings    = new VideoSettingsWindow;
  winAudioSettings    = new AudioSettingsWindow;
  winInputSettings    = new InputSettingsWindow;
  winPathSettings     = new PathSettingsWindow;
  winCheatEditor      = new CheatEditorWindow;
  winAdvancedSettings = new AdvancedSettingsWindow;
  winInputCapture     = new InputCaptureWindow;
  winCodeEditor       = new CodeEditorWindow;

  winVideoSettings->setup();
  winAudioSettings->setup();
  winInputSettings->setup();
  winPathSettings->setup();
  winCheatEditor->setup();
  winAdvancedSettings->setup();
  winInputCapture->setup();
  winCodeEditor->setup();

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(winVideoSettings->panel);
  panelLayout->addWidget(winAudioSettings->panel);
  panelLayout->addWidget(winInputSettings->panel);
  panelLayout->addWidget(winPathSettings->panel);
  panelLayout->addWidget(winCheatEditor->panel);
  panelLayout->addWidget(winAdvancedSettings->panel);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(listChanged()));

  listChanged();
  window->resize(600, 360);
}

void SettingsWindow::show() {
  window->show();

  static bool firstShow = true;
  if(firstShow == true) {
    firstShow = false;
    utility.centerWindow(window);
  }

  application.processEvents();
  window->activateWindow();
  window->raise();
}

void SettingsWindow::listChanged() {
  QListWidgetItem *item = list->currentItem();
  if(item == video)      panelLayout->setCurrentWidget(winVideoSettings->panel);
  if(item == audio)      panelLayout->setCurrentWidget(winAudioSettings->panel);
  if(item == input)      panelLayout->setCurrentWidget(winInputSettings->panel);
  if(item == paths)      panelLayout->setCurrentWidget(winPathSettings->panel);
  if(item == cheatcodes) panelLayout->setCurrentWidget(winCheatEditor->panel);
  if(item == advanced)   panelLayout->setCurrentWidget(winAdvancedSettings->panel);
}
