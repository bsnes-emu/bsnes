#include "cheateditor.cpp"
#include "cheatfinder.cpp"
#include "statemanager.cpp"

ToolsWindow::ToolsWindow() {
  window = new QbWindow(config.geometry.toolsWindow);
  window->setObjectName("tools-window");
  window->setWindowTitle("Tools");
  window->resize(625, 360);

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  window->setLayout(layout);

  splitter = new QSplitter;
  layout->addWidget(splitter);

  list = new QTreeWidget;
  list->addTopLevelItem(cheatEditor = new QTreeWidgetItem(QStringList() << "Cheat Editor"));
  list->addTopLevelItem(cheatFinder = new QTreeWidgetItem(QStringList() << "Cheat Finder"));
  list->addTopLevelItem(stateManager = new QTreeWidgetItem(QStringList() << "State Manager"));
  list->setCurrentItem(cheatEditor);
  list->setHeaderHidden(true);
  list->setRootIsDecorated(false);
  list->setAllColumnsShowFocus(true);
  list->setIconSize(QSize(22, 22));

  cheatEditor->setIcon(0, QIcon(":/22x22/accessories-text-editor.png"));
  cheatFinder->setIcon(0, QIcon(":/22x22/system-search.png"));
  stateManager->setIcon(0, QIcon(":/22x22/system-file-manager.png"));

  panel = new QWidget;
  panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  splitter->addWidget(list);
  splitter->addWidget(panel);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 5);

  cheatEditorWindow  = new CheatEditorWindow;
  cheatFinderWindow  = new CheatFinderWindow;
  stateManagerWindow = new StateManagerWindow;

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(cheatEditorWindow->panel);
  panelLayout->addWidget(cheatFinderWindow->panel);
  panelLayout->addWidget(stateManagerWindow->panel);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged()));

  itemChanged();
}

void ToolsWindow::itemChanged() {
  QTreeWidgetItem *item = list->currentItem();

  if(item == cheatEditor)  panelLayout->setCurrentWidget(cheatEditorWindow->panel);
  if(item == cheatFinder)  panelLayout->setCurrentWidget(cheatFinderWindow->panel);
  if(item == stateManager) panelLayout->setCurrentWidget(stateManagerWindow->panel);
}

void ToolsWindow::showCheatEditor() {
  list->setCurrentItem(cheatEditor);
  window->show();
}

void ToolsWindow::showCheatFinder() {
  list->setCurrentItem(cheatFinder);
  window->show();
}

void ToolsWindow::showStateManager() {
  list->setCurrentItem(stateManager);
  window->show();
}
