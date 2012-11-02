ScoreSettings *scoreSettings = nullptr;

ScoreSettings::ScoreSettings() {
  title.setFont(application->titleFont);
  title.setText("High Score Server Configuration");
  hostLabel.setText("Hostname:");
  userLabel.setText("Username:");
  passLabel.setText("Password:");

  unsigned width = max(
    Font(application->normalFont).geometry("Hostname:").width,
    Font(application->normalFont).geometry("Username:").width
  );

  append(title, {~0, 0}, 5);
  append(serverLayout, {~0, 0}, 5);
    serverLayout.append(hostLabel, {width, 0}, 5);
    serverLayout.append(hostEdit, {~0, 0});
  append(loginLayout, {~0, 0});
    loginLayout.append(userLabel, {width, 0}, 5);
    loginLayout.append(userEdit, {~0, 0}, 5);
    loginLayout.append(passLabel, {0, 0}, 5);
    loginLayout.append(passEdit, {~0, 0});

  hostEdit.setText(config->highScores.hostname);
  userEdit.setText(config->highScores.username);
  passEdit.setText(config->highScores.password);

  hostEdit.onChange = [&] { config->highScores.hostname = hostEdit.text(); };
  userEdit.onChange = [&] { config->highScores.username = userEdit.text(); };
  passEdit.onChange = [&] { config->highScores.password = passEdit.text(); };
}
