#include "interface.hpp"
Application *application = nullptr;
RuleEditor *ruleEditor = nullptr;

#include "resource/resource.hpp"
#include "resource/resource.cpp"

Application::Application(const string &pathname) : pathname(pathname) {
  setTitle("kaijuu v05");
  setFrameGeometry({64, 64, 725, 480});

  layout.setMargin(5);
  statusLabel.setFont("Tahoma, 8, Bold");
  uninstallButton.setText("Uninstall");
  installButton.setText("Install");
  settingList.setHeaderText("Name", "Default", "Match", "Pattern", "Command");
  settingList.setHeaderVisible();
  appendButton.setText("Append");
  modifyButton.setText("Modify");
  moveUpButton.setText("Move Up");
  moveDownButton.setText("Move Down");
  removeButton.setText("Remove");
  resetButton.setText("Reset");
  helpButton.setText("Help ...");

  append(layout);
  layout.append(installLayout, {~0, 0}, 5);
    installLayout.append(statusLabel, {~0, 0}, 5);
    installLayout.append(uninstallButton, {80, 0}, 5);
    installLayout.append(installButton, {80, 0});
  layout.append(settingLayout, {~0, ~0});
    settingLayout.append(settingList, {~0, ~0}, 5);
    settingLayout.append(controlLayout, {0, ~0});
      controlLayout.append(appendButton, {80, 0}, 5);
      controlLayout.append(modifyButton, {80, 0}, 5);
      controlLayout.append(moveUpButton, {80, 0}, 5);
      controlLayout.append(moveDownButton, {80, 0}, 5);
      controlLayout.append(removeButton, {80, 0}, 5);
      controlLayout.append(spacer, {0, ~0});
      controlLayout.append(resetButton, {80, 0}, 5);
      controlLayout.append(helpButton, {80, 0}, 5);
      controlLayout.append(canvas, {80, 88});

  canvas.setImage({resource::icon, sizeof resource::icon});
  canvas.update();

  onClose = &OS::quit;
  installButton.onActivate = {&Application::install, this};
  uninstallButton.onActivate = {&Application::uninstall, this};
  settingList.onActivate = {&Application::modifyAction, this};
  settingList.onChange = {&Application::synchronize, this};
  appendButton.onActivate = {&Application::appendAction, this};
  modifyButton.onActivate = {&Application::modifyAction, this};
  moveUpButton.onActivate = {&Application::moveUpAction, this};
  moveDownButton.onActivate = {&Application::moveDownAction, this};
  removeButton.onActivate = {&Application::removeAction, this};
  resetButton.onActivate = {&Application::resetAction, this};
  helpButton.onActivate = [&] { nall::invoke("kaijuu.html"); };
  refresh();
  synchronize();
  setVisible();
}

void Application::synchronize() {
  if(registry::read({"HKLM/Software/Microsoft/Windows/CurrentVersion/Shell Extensions/Approved/", classID}) == classDescription) {
    statusLabel.setText("Extension status: installed");
    installButton.setEnabled(false);
    uninstallButton.setEnabled(true);
  } else {
    statusLabel.setText("Extension status: not installed");
    installButton.setEnabled(true);
    uninstallButton.setEnabled(false);
  }
  modifyButton.setEnabled(settingList.selected());
  moveUpButton.setEnabled(settingList.selected() && settings.rules.size() > 1 && settingList.selection() != 0);
  moveDownButton.setEnabled(settingList.selected() && settings.rules.size() > 1 && settingList.selection() < settings.rules.size() - 1);
  removeButton.setEnabled(settingList.selected());
  resetButton.setEnabled(settings.rules.size() > 0);
}

void Application::refresh() {
  settings.load();
  settingList.reset();
  for(auto &rule : settings.rules) {
    string match = "Nothing";
    if(rule.matchFiles && rule.matchFolders) match = "Everything";
    else if(rule.matchFiles) match = "Files";
    else if(rule.matchFolders) match = "Folders";
    settingList.append(rule.name, rule.defaultAction ? "Yes" : "No", match, rule.pattern, rule.command);
  }
  settingList.autoSizeColumns();
}

void Application::install() {
  string command = {"regsvr32 \"", pathname, classDriver, "\""};
  _wsystem(utf16_t(command));
  synchronize();
}

void Application::uninstall() {
  string command = {"regsvr32 /u \"", pathname, classDriver, "\""};
  _wsystem(utf16_t(command));
  synchronize();
}

void Application::appendAction() {
  ruleEditor->show();
}

void Application::modifyAction() {
  if(settingList.selected() == false) return;
  unsigned selection = settingList.selection();
  ruleEditor->show(selection);
}

void Application::moveUpAction() {
  if(settingList.selected() == false) return;
  if(settingList.selection() == 0) return;
  unsigned selection = settingList.selection();
  auto temp = settings.rules(selection - 1);
  settings.rules(selection - 1) = settings.rules(selection);
  settings.rules(selection) = temp;
  settings.save();
  refresh();
  settingList.setSelection(selection - 1);
  synchronize();
}

void Application::moveDownAction() {
  if(settingList.selected() == false) return;
  if(settingList.selection() >= settings.rules.size() - 1) return;
  unsigned selection = settingList.selection();
  auto temp = settings.rules(selection + 1);
  settings.rules(selection + 1) = settings.rules(selection);
  settings.rules(selection) = temp;
  settings.save();
  refresh();
  settingList.setSelection(selection + 1);
  synchronize();
}

void Application::removeAction() {
  if(settingList.selected() == false) return;
  unsigned selection = settingList.selection();
  settings.rules.remove(selection);
  settings.save();
  refresh();
  synchronize();
}

void Application::resetAction() {
  if(MessageWindow::question(*this, "Warning: this will permanently remove all rules! Are you sure you want to do this?")
  == MessageWindow::Response::No) return;
  settings.rules.reset();
  settings.save();
  refresh();
  synchronize();
}

RuleEditor::RuleEditor() : index(-1) {
  setTitle("Rule Editor");
  setFrameGeometry({128, 256, 500, 160});

  layout.setMargin(5);
  nameLabel.setText("Name:");
  patternLabel.setText("Pattern:");
  commandLabel.setText("Command:");
  commandSelect.setText("Select ...");
  defaultAction.setText("Default Action");
  filesAction.setText("Match Files");
  foldersAction.setText("Match Folders");
  assignButton.setText("Assign");

  Font font("Tahoma, 8");
  unsigned length = 0;
  length = max(length, font.geometry("Name:").width);
  length = max(length, font.geometry("Pattern:").width);
  length = max(length, font.geometry("Command:").width);

  append(layout);
  layout.append(nameLayout, {~0, 0}, 5);
    nameLayout.append(nameLabel, {length, 0}, 5);
    nameLayout.append(nameValue, {~0, 0});
  layout.append(patternLayout, {~0, 0}, 5);
    patternLayout.append(patternLabel, {length, 0}, 5);
    patternLayout.append(patternValue, {~0, 0});
  layout.append(commandLayout, {~0, 0}, 5);
    commandLayout.append(commandLabel, {length, 0}, 5);
    commandLayout.append(commandValue, {~0, 0}, 5);
    commandLayout.append(commandSelect, {80, 0});
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(defaultAction, {0, 0}, 5);
    controlLayout.append(filesAction, {0, 0}, 5);
    controlLayout.append(foldersAction, {0, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(assignButton, {80, 0});

  Geometry geometry = Window::geometry();
  geometry.height = layout.minimumGeometry().height;
  setGeometry(geometry);

  onClose = [&] {
    modal = false;
  };

  nameValue.onChange =
  patternValue.onChange =
  commandValue.onChange =
  {&RuleEditor::synchronize, this};

  commandSelect.onActivate = [&] {
    string pathname = DialogWindow::fileOpen(*this, application->pathname, "Programs (*.exe)", "All Files (*)");
    if(pathname.empty() == false) {
      pathname.transform("/", "\\");
      commandValue.setText({"\"", pathname, "\" {file}"});
    }
    synchronize();
  };

  nameValue.onActivate =
  patternValue.onActivate =
  commandValue.onActivate =
  assignButton.onActivate = [&] {
    Settings::Rule rule = {
      nameValue.text(),
      patternValue.text(),
      defaultAction.checked(),
      filesAction.checked(),
      foldersAction.checked(),
      commandValue.text()
    };
    if(index == -1) {
      settings.rules.append(rule);
    } else {
      settings.rules(index) = rule;
    }
    settings.save();
    application->refresh();
    application->synchronize();
    setVisible(false);
    modal = false;
  };
}

void RuleEditor::synchronize() {
  bool enable = true;
  if(nameValue.text().empty()) enable = false;
  if(patternValue.text().empty()) enable = false;
  if(commandValue.text().empty()) enable = false;
  assignButton.setEnabled(enable);
}

void RuleEditor::show(signed ruleID) {
  Settings::Rule rule{"", "", false, true, false, "", false};
  if(ruleID >= 0) rule = settings.rules(ruleID);

  index = ruleID;
  nameValue.setText(rule.name);
  patternValue.setText(rule.pattern);
  commandValue.setText(rule.command);
  defaultAction.setChecked(rule.defaultAction);
  filesAction.setChecked(rule.matchFiles);
  foldersAction.setChecked(rule.matchFolders);
  synchronize();
  setVisible();
  setFocused();
  nameValue.setFocused();

  setModal(modal = true);
  while(modal == true) {
    OS::processEvents();
  }
  setModal(false);
  application->setFocused();
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

bool isWow64() {
  LPFN_ISWOW64PROCESS fnIsWow64Process;
  fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process");
  if(!fnIsWow64Process) return false;
  BOOL result = FALSE;
  fnIsWow64Process(GetCurrentProcess(), &result);
  return result == TRUE;
}

int CALLBACK WinMain(HINSTANCE module, HINSTANCE, LPSTR, int) {
  if(isWow64()) {
    MessageWindow::critical(Window::none(), "Error: you must run kaijuu64.exe on 64-bit Windows.");
    return 0;
  }

  wchar_t filename[MAX_PATH];
  GetModuleFileNameW(module, filename, MAX_PATH);

  string pathname = (const char*)utf8_t(filename);
  pathname.transform("\\", "/");
  pathname = dir(pathname);

  application = new Application(pathname);
  ruleEditor = new RuleEditor;
  application->setFocused();
  OS::main();

  return 0;
}
