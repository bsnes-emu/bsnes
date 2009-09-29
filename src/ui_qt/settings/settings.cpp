#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "paths.cpp"
#include "advanced.cpp"
#include "utility/inputcapture.cpp"

SettingsWindow::SettingsWindow() {
  setObjectName("settings-window");
  setWindowTitle("Configuration Settings");
  setMinimumSize(625, 360);

  //use QTreeWidget instead of QListWidget, as only the former has setAllColumnsShowFocus()
  //this is needed to have the dotted-selection rectangle encompass the icons
  list = new QTreeWidget;
  list->addTopLevelItem(video = new QTreeWidgetItem(QStringList() << "Video"));
  list->addTopLevelItem(audio = new QTreeWidgetItem(QStringList() << "Audio"));
  list->addTopLevelItem(input = new QTreeWidgetItem(QStringList() << "Input"));
  list->addTopLevelItem(paths = new QTreeWidgetItem(QStringList() << "Paths"));
  list->addTopLevelItem(advanced = new QTreeWidgetItem(QStringList() << "Advanced"));
  list->setCurrentItem(input);  //select most frequently used panel by default
  list->setFixedWidth(135);
  list->setHeaderHidden(true);
  list->setRootIsDecorated(false);
  list->setAllColumnsShowFocus(true);
  list->setIconSize(QSize(22, 22));

  video->setIcon(0, QIcon(":/22x22/video-display.png"));
  audio->setIcon(0, QIcon(":/22x22/audio-volume-high.png"));
  input->setIcon(0, QIcon(":/22x22/input-gaming.png"));
  paths->setIcon(0, QIcon(":/22x22/folder.png"));
  advanced->setIcon(0, QIcon(":/22x22/preferences-system.png"));

  panel = new QWidget;
  panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  layout->addWidget(list);
  layout->addWidget(panel);
  setLayout(layout);

  winVideoSettings    = new VideoSettingsWindow;
  winAudioSettings    = new AudioSettingsWindow;
  winInputSettings    = new InputSettingsWindow;
  winPathSettings     = new PathSettingsWindow;
  winAdvancedSettings = new AdvancedSettingsWindow;
  winInputCapture     = new InputCaptureWindow;

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(winVideoSettings->panel);
  panelLayout->addWidget(winAudioSettings->panel);
  panelLayout->addWidget(winInputSettings->panel);
  panelLayout->addWidget(winPathSettings->panel);
  panelLayout->addWidget(winAdvancedSettings->panel);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged()));

  itemChanged();
}

void SettingsWindow::itemChanged() {
  QTreeWidgetItem *item = list->currentItem();

  if(item == video)    panelLayout->setCurrentWidget(winVideoSettings->panel);
  if(item == audio)    panelLayout->setCurrentWidget(winAudioSettings->panel);
  if(item == input)    panelLayout->setCurrentWidget(winInputSettings->panel);
  if(item == paths)    panelLayout->setCurrentWidget(winPathSettings->panel);
  if(item == advanced) panelLayout->setCurrentWidget(winAdvancedSettings->panel);
}
