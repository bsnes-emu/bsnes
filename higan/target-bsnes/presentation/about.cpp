AboutWindow::AboutWindow() {
  aboutWindow = this;

  setTitle({tr("About {0}", "bsnes"), " ..."});
  setBackgroundColor({255, 255, 240});
  layout.setPadding(10);
  auto logo = image{Resource::Logo};
  logo.alphaBlend(0xfffff0);
  canvas.setIcon(logo);
  tableLayout.setFont(Font().setBold());
  tableLayout.setSize({2, 4});
  tableLayout.column(0).setSpacing(4);
  tableLayout.row(0).setSpacing(2);
  tableLayout.row(1).setSpacing(2);
  tableLayout.row(2).setSpacing(2);
  versionLabel.setText({tr("Version"), ":"}).setAlignment(1.0);
  versionValue.setText(Emulator::Version);
  authorLabel.setText({tr("Author"), ":"}).setAlignment(1.0);
  authorValue.setText(Emulator::Author);
  licenseLabel.setText({tr("License"), ":"}).setAlignment(1.0);
  licenseValue.setText(Emulator::License);
  websiteLabel.setText({tr("Website"), ":"}).setAlignment(1.0);
  websiteValue.setText(Emulator::Website);

  setResizable(false);
  setSize(layout.minimumSize());
  setDismissable();
}
