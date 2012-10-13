PathSettings *pathSettings = nullptr;

PathSettings::PathSettings() {
  title.setFont(application->titleFont);
  title.setText("Path Settings");
  gameLabel.setText("Games:");
  gamePath.setEditable(false);
  gamePath.setText(config->path.game);
  gameBrowse.setText("Browse ...");

  append(title, {~0, 0}, 5);
  append(gameLayout, {~0, 0});
    gameLayout.append(gameLabel, {0, 0}, 5);
    gameLayout.append(gamePath, {~0, 0}, 5);
    gameLayout.append(gameBrowse, {80, 0});

  gameBrowse.onActivate = [&] {
    string path = DialogWindow::folderSelect(*settings, userpath());
    if(path.empty() == false) gamePath.setText(config->path.game = path);
  };
}
