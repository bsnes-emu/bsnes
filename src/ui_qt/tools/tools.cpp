#include "../ui-base.hpp"

#include "tools.moc"
ToolsWindow *toolsWindow;

#include "cheateditor.cpp"
#include "cheatfinder.cpp"
#include "statemanager.cpp"

ToolsWindow::ToolsWindow() {
  setObjectName("tools-window");
  setWindowTitle("Tools");
  resize(600, 360);
  setGeometryString(&config().geometry.toolsWindow);
  application.windowList.append(this);

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  cheatEditorWindow = new CheatEditorWindow;
  cheatFinderWindow = new CheatFinderWindow;
  stateManagerWindow = new StateManagerWindow;

  cheatEditorArea = new QScrollArea;
  cheatEditorArea->setWidget(cheatEditorWindow);
  cheatEditorArea->setFrameStyle(0);
  cheatEditorArea->setWidgetResizable(true);

  cheatFinderArea = new QScrollArea;
  cheatFinderArea->setWidget(cheatFinderWindow);
  cheatFinderArea->setFrameStyle(0);
  cheatFinderArea->setWidgetResizable(true);

  stateManagerArea = new QScrollArea;
  stateManagerArea->setWidget(stateManagerWindow);
  stateManagerArea->setFrameStyle(0);
  stateManagerArea->setWidgetResizable(true);

  tab = new QTabWidget;
  tab->addTab(cheatEditorArea, QIcon(":/16x16/accessories-text-editor.png"), "Cheat Editor");
  tab->addTab(cheatFinderArea, QIcon(":/16x16/system-search.png"), "Cheat Finder");
  tab->addTab(stateManagerArea, QIcon(":/16x16/system-file-manager.png"), "State Manager");
  layout->addWidget(tab);
}
