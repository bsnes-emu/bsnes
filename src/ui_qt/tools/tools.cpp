#include "cheateditor.cpp"
#include "statemanager.cpp"

ToolsWindow::ToolsWindow() {
  setObjectName("tools-window");
  setWindowTitle("Tools");
  setMinimumSize(625, 360);

  list = new QTreeWidget;
  list->addTopLevelItem(cheatEditor = new QTreeWidgetItem(QStringList() << "Cheat Editor"));
  list->addTopLevelItem(stateManager = new QTreeWidgetItem(QStringList() << "State Manager"));
  list->setCurrentItem(cheatEditor);
  list->setFixedWidth(135);
  list->setHeaderHidden(true);
  list->setRootIsDecorated(false);
  list->setAllColumnsShowFocus(true);
  list->setIconSize(QSize(22, 22));
  cheatEditor->setIcon(0, QIcon(":/22x22/accessories-text-editor.png"));
  stateManager->setIcon(0, QIcon(":/22x22/system-file-manager.png"));

  panel = new QWidget;
  panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  layout->addWidget(list);
  layout->addWidget(panel);
  setLayout(layout);

  cheatEditorWindow = new CheatEditorWindow;
  stateManagerWindow = new StateManagerWindow;

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(cheatEditorWindow->panel);
  panelLayout->addWidget(stateManagerWindow->panel);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged()));

  itemChanged();
}

void ToolsWindow::itemChanged() {
  QTreeWidgetItem *item = list->currentItem();

  if(item == cheatEditor)  panelLayout->setCurrentWidget(cheatEditorWindow->panel);
  if(item == stateManager) panelLayout->setCurrentWidget(stateManagerWindow->panel);
}

void ToolsWindow::showCheatEditor() {
  list->setCurrentItem(cheatEditor);
  showAt(1.0, 1.0);
  setFocus();
}

void ToolsWindow::showStateManager() {
  list->setCurrentItem(stateManager);
  showAt(1.0, 1.0);
  setFocus();
}
