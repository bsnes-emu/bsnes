//center specified top-level window onscreen:
//accounts for taskbar, dock, window frame, etc.
void Utility::centerWindow(QWidget *window) {
  QRect deskRect = QApplication::desktop()->availableGeometry();
  unsigned deskWidth  = (deskRect.right() - deskRect.left() + 1);
  unsigned deskHeight = (deskRect.bottom() - deskRect.top() + 1);

  if(window->isVisible()) {
    QRect windowRect = window->frameGeometry();
    unsigned windowWidth  = (windowRect.right() - windowRect.left() + 1);
    unsigned windowHeight = (windowRect.bottom() - windowRect.top() + 1);

    unsigned x = (deskWidth  >= windowWidth ) ? (deskWidth  - windowWidth ) / 2 : 0;
    unsigned y = (deskHeight >= windowHeight) ? (deskHeight - windowHeight) / 2 : 0;
    window->move(deskRect.left() + x, deskRect.top() + y);
  } else {
    unsigned x = (deskWidth  >= window->size().width() ) ? (deskWidth  - window->size().width() ) / 2 : 0;
    unsigned y = (deskHeight >= window->size().height()) ? (deskHeight - window->size().height()) / 2 : 0;
    window->setGeometry(
      deskRect.left() + x, deskRect.top() + y,
      window->size().width(), window->size().height()
    );
  }
}

void Utility::updateFullscreenState() {
  video.clear();

  if(config.video.isFullscreen == false) {
    config.video.context = &config.video.windowed;
    winMain->window->showNormal();
  } else {
    config.video.context = &config.video.fullscreen;
    winMain->window->showFullScreen();
  }

  //Xlib requires time to propogate fullscreen state change message to window manager;
  //if window is resized before this occurs, canvas may not resize correctly
  application.processEvents();
  usleep(50000);

  //refresh options that are unique to each video context
  resizeMainWindow();
  updateVideoMode();
  updateHardwareFilter();
  updateSoftwareFilter();
  winMain->syncUi();
}

//if max exceeds x: x is set to max, and y is scaled down to keep proportion to x
void Utility::constrainSize(unsigned &x, unsigned &y, unsigned max) {
  if(x > max) {
    double scalar = (double)max / (double)x;
    y = (unsigned)((double)y * (double)scalar);
    x = max;
  }
}

void Utility::resizeMainWindow() {
  //Xlib requires time to propogate window messages to window manager;
  //repeat resize a few times to ensure window ends up correctly sized and centered
  for(unsigned i = 0; i < 10; i++) {
    unsigned multiplier = config.video.context->multiplier;
    unsigned width  = 256 * config.video.context->multiplier;
    unsigned height = (config.video.context->region == 0 ? 224 : 239) * config.video.context->multiplier;

    if(config.video.context->correctAspectRatio) {
      if(config.video.context->region == 0) {
        width = (double)width * config.video.ntscAspectRatio + 0.5;  //NTSC adjust
      } else {
        width = (double)width * config.video.palAspectRatio  + 0.5;  //PAL adjust
      }
    }

    QDesktopWidget *desktop = QApplication::desktop();
    application.processEvents();

    if(config.video.isFullscreen == false) {
      //get effective desktop work area region (ignore Windows taskbar, OS X doc, etc.)
      QRect deskRect = desktop->availableGeometry();
      unsigned deskWidth  = (deskRect.right() - deskRect.left() + 1);
      unsigned deskHeight = (deskRect.bottom() - deskRect.top() + 1);

      //calculate frame geometry (window border + menubar + statusbar)
      unsigned frameWidth, frameHeight;
      if(winMain->window->isVisible()) {
        QRect frameRect = winMain->window->frameGeometry();
        frameWidth  = (frameRect.right() - frameRect.left() + 1) - winMain->canvasContainer->size().width();
        frameHeight = (frameRect.bottom() - frameRect.top() + 1) - winMain->canvasContainer->size().height();
      } else {
        //frameGeometry() is inaccurate when window is not visible
        //(especially before it is shown for the first time)
        frameWidth  = 10;  //use reasonable defaults
        frameHeight = 80;  //for frame size
      }

      //ensure window size will not be larger than viewable desktop area
      constrainSize(height, width, deskHeight - frameHeight);
      constrainSize(width, height, deskWidth  - frameWidth );

      //resize window such that it is as small as possible to hold canvas
      //of size (width, height); and center resultant window onscreen
      winMain->canvas->setFixedSize(width, height);
      winMain->window->move(
        deskRect.left() + ((deskWidth  - (frameWidth  + width )) / 2),
        deskRect.top () + ((deskHeight - (frameHeight + height)) / 2)
      );
      winMain->window->resize(width, height);
    } else {
      //center canvas onscreen, ensure it is not larger than viewable area
      winMain->canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      constrainSize(height, width, winMain->canvasContainer->size().height());
      constrainSize(width, height, winMain->canvasContainer->size().width());
      winMain->canvas->setMaximumSize(width, height);
    }

    application.processEvents();
    usleep(2000);
  }

  //work around for Qt/Xlib bug:
  //if window resize occurs with cursor over it, Qt shows Qt::Size*DiagCursor;
  //so force it to show Qt::ArrowCursor, as expected
  winMain->window->setCursor(Qt::ArrowCursor);
  winMain->canvasContainer->setCursor(Qt::ArrowCursor);
  winMain->canvas->setCursor(Qt::ArrowCursor);
}
