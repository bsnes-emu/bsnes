VideoWindow *videoWindow = nullptr;

VideoWindow::VideoWindow() {
  setTitle("ethos");
  setGeometry({1024, 600, 720, 480});
  setBackgroundColor({0, 0, 0});

  append(layout);
  layout.append(viewport, {0, 0, 720, 480});

  onClose = [&] { application->quit = true; };
}
