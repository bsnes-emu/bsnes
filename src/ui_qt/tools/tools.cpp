#include "../ui-base.hpp"

#include "tools.moc"
ToolsWindow *toolsWindow;

#include "cheateditor.cpp"
#include "cheatfinder.cpp"
#include "statemanager.cpp"
#include "layertoggle.cpp"

ToolsWindow::ToolsWindow() : QbWindow(config().geometry.toolsWindow) {
  setObjectName("tools-window");
  setWindowTitle("Tools");
  resize(625, 360);

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  splitter = new QSplitter;
  layout->addWidget(splitter);

  list = new QTreeWidget;
  list->addTopLevelItem(cheatEditor = new QTreeWidgetItem(QStringList() << "Cheat Editor"));
  list->addTopLevelItem(cheatFinder = new QTreeWidgetItem(QStringList() << "Cheat Finder"));
  list->addTopLevelItem(stateManager = new QTreeWidgetItem(QStringList() << "State Manager"));
  list->addTopLevelItem(layerToggle = new QTreeWidgetItem(QStringList() << "Layer Toggle"));
  list->setCurrentItem(cheatEditor);
  list->setHeaderHidden(true);
  list->setRootIsDecorated(false);
  list->setAllColumnsShowFocus(true);
  list->setIconSize(QSize(22, 22));

  cheatEditor->setIcon(0, QIcon(":/22x22/accessories-text-editor.png"));
  cheatFinder->setIcon(0, QIcon(":/22x22/system-search.png"));
  stateManager->setIcon(0, QIcon(":/22x22/system-file-manager.png"));
  layerToggle->setIcon(0, QIcon(":/22x22/image-x-generic.png"));

  panel = new QWidget;
  panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  splitter->addWidget(list);
  splitter->addWidget(panel);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 5);

  cheatEditorWindow = new CheatEditorWindow;
  cheatFinderWindow = new CheatFinderWindow;
  stateManagerWindow = new StateManagerWindow;
  layerToggleWindow = new LayerToggleWindow;

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(cheatEditorWindow);
  panelLayout->addWidget(cheatFinderWindow);
  panelLayout->addWidget(stateManagerWindow);
  panelLayout->addWidget(layerToggleWindow);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged()));

  itemChanged();
}

void ToolsWindow::itemChanged() {
  QTreeWidgetItem *item = list->currentItem();

  if(item == cheatEditor) panelLayout->setCurrentWidget(cheatEditorWindow);
  if(item == cheatFinder) panelLayout->setCurrentWidget(cheatFinderWindow);
  if(item == stateManager) panelLayout->setCurrentWidget(stateManagerWindow);
  if(item == layerToggle) panelLayout->setCurrentWidget(layerToggleWindow);
}
