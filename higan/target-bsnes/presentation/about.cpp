AboutWindow::AboutWindow() {
  aboutWindow = this;

  setTitle("About bsnes ...");
  setBackgroundColor({255, 255, 240});
  layout.setMargin(10);
  auto logo = image{Resource::Logo::bsnes};
  logo.alphaBlend(0xfffff0);
  canvas.setIcon(logo);
  informationLeft.setFont(Font().setBold()).setAlignment(1.0).setText({
    "Version:\n",
    "Author:\n",
    "License:\n",
    "Website:"
  });
  informationRight.setFont(Font().setBold()).setAlignment(0.0).setText({
    Emulator::Version, "\n",
    Emulator::Author, "\n",
    Emulator::License, "\n",
    Emulator::Website
  });

  setResizable(false);
  setSize(layout.minimumSize());
  setDismissable();
}
