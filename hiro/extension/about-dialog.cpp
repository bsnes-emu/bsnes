#if defined(Hiro_AboutDialog)

auto AboutDialog::setAuthor(const string& author) -> type& {
  state.author = author;
  return *this;
}

auto AboutDialog::setDescription(const string& description) -> type& {
  state.description = description;
  return *this;
}

auto AboutDialog::setLicense(const string& license) -> type& {
  state.license = license;
  return *this;
}

auto AboutDialog::setLogo(const image& logo) -> type& {
  state.logo = logo;
  state.logo.transform();
  state.logo.alphaBlend(0xfffff0);
  return *this;
}

auto AboutDialog::setName(const string& name) -> type& {
  state.name = name;
  return *this;
}

auto AboutDialog::setParent(sWindow parent) -> type& {
  state.parent = parent;
  return *this;
}

auto AboutDialog::setVersion(const string& version) -> type& {
  state.version = version;
  return *this;
}

auto AboutDialog::setWebsite(const string& website) -> type& {
  state.website = website;
  return *this;
}

auto AboutDialog::show() -> void {
  Window window;
  window.onClose([&] { window.setModal(false); });

  VerticalLayout layout{&window};
  layout.setPadding(5);

  Label nameLabel{&layout, Size{~0, 0}};
  nameLabel.setCollapsible();
  nameLabel.setAlignment(0.5);
  nameLabel.setForegroundColor({0, 0, 0});
  nameLabel.setFont(Font().setFamily("Georgia").setBold().setSize(36.0));
  nameLabel.setText(state.name ? state.name : Application::name());
  nameLabel.setVisible(!state.logo);

  Canvas logoCanvas{&layout, Size{~0, 0}};
  logoCanvas.setCollapsible();
  logoCanvas.setIcon(state.logo);
  logoCanvas.setVisible((bool)state.logo);

  Label descriptionLabel{&layout, Size{~0, 0}};
  descriptionLabel.setCollapsible();
  descriptionLabel.setAlignment(0.5);
  descriptionLabel.setForegroundColor({0, 0, 0});
  descriptionLabel.setText(state.description);
  if(!state.description) descriptionLabel.setVisible(false);

  HorizontalLayout versionLayout{&layout, Size{~0, 0}, 0};
  versionLayout.setCollapsible();
  Label versionLabel{&versionLayout, Size{~0, 0}, 3};
  versionLabel.setAlignment(1.0);
  versionLabel.setFont(Font().setBold());
  versionLabel.setForegroundColor({0, 0, 0});
  versionLabel.setText("Version:");
  Label versionValue{&versionLayout, Size{~0, 0}};
  versionValue.setAlignment(0.0);
  versionValue.setFont(Font().setBold());
  versionValue.setForegroundColor({0, 0, 0});
  versionValue.setText(state.version);
  if(!state.version) versionLayout.setVisible(false);

  HorizontalLayout authorLayout{&layout, Size{~0, 0}, 0};
  authorLayout.setCollapsible();
  Label authorLabel{&authorLayout, Size{~0, 0}, 3};
  authorLabel.setAlignment(1.0);
  authorLabel.setFont(Font().setBold());
  authorLabel.setForegroundColor({0, 0, 0});
  authorLabel.setText("Author:");
  Label authorValue{&authorLayout, Size{~0, 0}};
  authorValue.setAlignment(0.0);
  authorValue.setFont(Font().setBold());
  authorValue.setForegroundColor({0, 0, 0});
  authorValue.setText(state.author);
  if(!state.author) authorLayout.setVisible(false);

  HorizontalLayout licenseLayout{&layout, Size{~0, 0}, 0};
  licenseLayout.setCollapsible();
  Label licenseLabel{&licenseLayout, Size{~0, 0}, 3};
  licenseLabel.setAlignment(1.0);
  licenseLabel.setFont(Font().setBold());
  licenseLabel.setForegroundColor({0, 0, 0});
  licenseLabel.setText("License:");
  Label licenseValue{&licenseLayout, Size{~0, 0}};
  licenseValue.setAlignment(0.0);
  licenseValue.setFont(Font().setBold());
  licenseValue.setForegroundColor({0, 0, 0});
  licenseValue.setText(state.license);
  if(!state.license) licenseLayout.setVisible(false);

  HorizontalLayout websiteLayout{&layout, Size{~0, 0}, 0};
  websiteLayout.setCollapsible();
  Label websiteLabel{&websiteLayout, Size{~0, 0}, 3};
  websiteLabel.setAlignment(1.0);
  websiteLabel.setFont(Font().setBold());
  websiteLabel.setForegroundColor({0, 0, 0});
  websiteLabel.setText("Website:");
  Label websiteValue{&websiteLayout, Size{~0, 0}};
  websiteValue.setAlignment(0.0);
  websiteValue.setFont(Font().setBold());
  websiteValue.setForegroundColor({0, 0, 240});
  websiteValue.setText(state.website);
  websiteValue.onMouseRelease([&](Mouse::Button button) {
    if(button == Mouse::Button::Left) invoke(state.website);
  });
  if(!state.website) websiteLayout.setVisible(false);

  window.setTitle({"About ", state.name ? state.name : Application::name(), " ..."});
  window.setBackgroundColor({255, 255, 240});
  window.setSize({max(360, layout.minimumSize().width()), layout.minimumSize().height()});
  window.setResizable(false);
  window.setCentered(state.parent);
  window.setDismissable();
  window.setVisible();
  window.setModal();
}

#endif
