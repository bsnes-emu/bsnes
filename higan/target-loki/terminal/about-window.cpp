unique_pointer<AboutWindow> aboutWindow;

AboutWindow::AboutWindow() {
  aboutWindow = this;

  layout.setMargin(5);
  canvas.setIcon({locate("loki.png")});
  information.setFont(Font().setFamily(Font::Sans).setBold()).setAlignment(0.5).setText({
    "loki v", Emulator::Version, "\n\n"
    "Author: ", Emulator::Author, "\n",
    "License: ", Emulator::License, "\n",
    "Website: ", Emulator::Website
  });

  setTitle("About loki ...");
  setSize(layout.minimumSize());
  setCentered();
}
