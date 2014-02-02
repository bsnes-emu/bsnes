ServerSettings* serverSettings = nullptr;

ServerSettings::ServerSettings() {
  hostLabel.setText("Hostname:");
  userLabel.setText("Username:");
  passLabel.setText("Password:");

  unsigned width = min(
    Font::size(program->normalFont, "Hostname:").width,
    Font::size(program->normalFont, "Username:").width
  );

  append(hostLayout, {~0, 0}, 5);
    hostLayout.append(hostLabel, {width, 0}, 5);
    hostLayout.append(hostEdit, {~0, 0});
  append(userLayout, {~0, 0});
    userLayout.append(userLabel, {width, 0}, 5);
    userLayout.append(userEdit, {~0, 0}, 5);
    userLayout.append(passLabel, {0, 0}, 5);
    userLayout.append(passEdit, {~0, 0});

  hostEdit.setText(config->server.hostname);
  userEdit.setText(config->server.username);
  passEdit.setText(config->server.password);

  hostEdit.onChange = [&] { config->server.hostname = hostEdit.text(); };
  userEdit.onChange = [&] { config->server.username = userEdit.text(); };
  passEdit.onChange = [&] { config->server.password = passEdit.text(); };
}
