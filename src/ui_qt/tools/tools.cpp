#include "cheateditor.cpp"
#include "statemanager.cpp"

ToolsWindow::ToolsWindow() {
  setObjectName("tools-window");
  setWindowTitle("Tools");
  setMinimumSize(625, 360);

  list = new QListWidget;
  list->addItem(cheatEditor = new QListWidgetItem("Cheat Editor"));
  list->addItem(stateManager = new QListWidgetItem("State Manager"));
  list->setCurrentItem(cheatEditor);
  list->setFixedWidth(135);

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

  connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(listChanged()));

  listChanged();
}

void ToolsWindow::listChanged() {
  QListWidgetItem *item = list->currentItem();

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
