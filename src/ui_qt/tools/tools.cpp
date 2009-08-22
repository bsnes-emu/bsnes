#include "cheateditor.cpp"
#include "statemanager.cpp"

void ToolsWindow::setup() {
  window = new QWidget;
  window->setObjectName("tools-window");
  window->setWindowTitle("Tools");

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
  window->setLayout(layout);

  cheatEditorWindow = new CheatEditorWindow;
  stateManagerWindow = new StateManagerWindow;

  cheatEditorWindow->setup();
  stateManagerWindow->setup();

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(cheatEditorWindow->panel);
  panelLayout->addWidget(stateManagerWindow->panel);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(listChanged()));

  listChanged();
  window->setMinimumSize(625, 360);
}

void ToolsWindow::listChanged() {
  QListWidgetItem *item = list->currentItem();

  if(item == cheatEditor)  panelLayout->setCurrentWidget(cheatEditorWindow->panel);
  if(item == stateManager) panelLayout->setCurrentWidget(stateManagerWindow->panel);
}

void ToolsWindow::showCheatEditor() {
  list->setCurrentItem(cheatEditor);
  utility.showCentered(window);
}

void ToolsWindow::showStateManager() {
  list->setCurrentItem(stateManager);
  utility.showCentered(window);
}
