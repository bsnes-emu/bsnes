void Utility::updateFullscreenState() {
  video.clear();

  if(config().video.isFullscreen == false) {
    config().video.context = &config().video.windowed;
    mainWindow->showNormal();
    mainWindow->menuBar->setVisible(true);
    mainWindow->statusBar->setVisible(true);
  } else {
    config().video.context = &config().video.fullscreen;
    mainWindow->showFullScreen();
    mainWindow->menuBar->setVisible(false);
    mainWindow->statusBar->setVisible(false);
  }

  QApplication::processEvents();
  #if defined(PLATFORM_X)
  //Xlib requires time to propogate fullscreen state change message to window manager;
  //if window is resized before this occurs, canvas may not resize correctly
  usleep(50000);
  #endif

  //refresh options that are unique to each video context
  updateVideoMode();
  updateHardwareFilter();
  updateSoftwareFilter();
  mainWindow->syncUi();
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
  //process all pending events to ensure window size is correct (after fullscreen state change, etc)
  usleep(2000);
  QApplication::processEvents();

  unsigned screenWidth, screenHeight;
  if(config().video.isFullscreen == false) {
    screenWidth = QApplication::desktop()->availableGeometry(mainWindow).width();
    screenHeight = QApplication::desktop()->availableGeometry(mainWindow).height();
  } else {
    screenWidth = mainWindow->canvasContainer->size().width();
    screenHeight = mainWindow->canvasContainer->size().height();
  }

  unsigned region = config().video.context->region;
  unsigned multiplier = config().video.context->multiplier;
  unsigned &width = display.outputWidth;
  unsigned &height = display.outputHeight;
  width = 256 * multiplier;
  height = (region == 0 ? 224 : 239) * multiplier;

  if(config().video.context->correctAspectRatio) {
    if(region == 0) {
      width = (double)width * config().video.ntscAspectRatio + 0.5;  //NTSC adjust
    } else {
      width = (double)width * config().video.palAspectRatio  + 0.5;  //PAL adjust
    }
  }

  display.cropLeft = config().video.cropLeft;
  display.cropTop = config().video.cropTop;
  display.cropRight = config().video.cropRight;
  display.cropBottom = config().video.cropBottom;

  //ensure window size will not be larger than viewable desktop area
  constrainSize(height, width, screenHeight);
  constrainSize(width, height, screenWidth);

  if(config().video.isFullscreen == false) {
    mainWindow->canvas->setFixedSize(width, height);
    mainWindow->show();
  } else {
    if(multiplier == 6) {
      //Scale Max - Normal
      width = (double)width * (double)screenHeight / (double)height;
      height = screenHeight;
    }

    if(multiplier == 7) {
      //Scale Max - Wide
      width = screenWidth;
      height = screenHeight;
    }

    if(multiplier == 8) {
      //Scale Max - Wide Zoom
      //1. scale width and height proportionally until width of screen is completely filled
      //2. determine how much height goes out of screen by
      //3. cut half of the above value out of the visible input display region
      //this results in a 50% compromise between correct aspect ratio and fill mode;
      //while cropping out only 50% as much height as a fully proportional zoom would
      width = (double)width * (double)screenHeight / (double)height;
      height = screenHeight;
      unsigned widthDifference = screenWidth - width;
      unsigned adjustedHeight = (double)height * (double)(width + widthDifference) / (double)(width);
      unsigned heightDifference = adjustedHeight - height;
      width = screenWidth;
      display.cropLeft = 0;
      display.cropTop = 100.0 / (double)height * (heightDifference / 4);
      display.cropRight = 0;
      display.cropBottom = 100.0 / (double)height * (heightDifference / 4);
    }

    mainWindow->canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWindow->canvas->setFixedSize(width, height);
    mainWindow->canvas->setMinimumSize(0, 0);
  }

  //workaround for Qt/Xlib bug:
  //if window resize occurs with cursor over it, Qt shows Qt::Size*DiagCursor;
  //so force it to show Qt::ArrowCursor, as expected
  mainWindow->setCursor(Qt::ArrowCursor);
  mainWindow->canvasContainer->setCursor(Qt::ArrowCursor);
  mainWindow->canvas->setCursor(Qt::ArrowCursor);

  //workaround for DirectSound(?) bug:
  //window resizing sometimes breaks audio sync, this call re-initializes it
  updateAvSync();
}

void Utility::toggleSynchronizeVideo() {
  mainWindow->settings_emulationSpeed_syncVideo->toggleChecked();
  config().video.synchronize = mainWindow->settings_emulationSpeed_syncVideo->isChecked();
  updateAvSync();
}

void Utility::toggleSynchronizeAudio() {
  mainWindow->settings_emulationSpeed_syncAudio->toggleChecked();
  config().audio.synchronize = mainWindow->settings_emulationSpeed_syncAudio->isChecked();
  updateAvSync();
}

void Utility::setNtscMode() {
  config().video.context->region = 0;
  updateVideoMode();
  resizeMainWindow();
  mainWindow->shrink();
  mainWindow->syncUi();
}

void Utility::setPalMode() {
  config().video.context->region = 1;
  updateVideoMode();
  resizeMainWindow();
  mainWindow->shrink();
  mainWindow->syncUi();
}

void Utility::toggleSmoothVideoOutput() {
  mainWindow->settings_smoothVideo->toggleChecked();
  config().video.context->hwFilter = mainWindow->settings_smoothVideo->isChecked();
  updateHardwareFilter();
  mainWindow->syncUi();
}

void Utility::toggleAspectCorrection() {
  mainWindow->settings_videoMode_correctAspectRatio->toggleChecked();
  config().video.context->correctAspectRatio = mainWindow->settings_videoMode_correctAspectRatio->isChecked();
  resizeMainWindow();
  mainWindow->shrink();
}

void Utility::setScale(unsigned scale) {
  config().video.context->multiplier = scale;
  resizeMainWindow();
  mainWindow->shrink();
  mainWindow->syncUi();
}

void Utility::toggleFullscreen() {
  config().video.isFullscreen = !config().video.isFullscreen;
  updateFullscreenState();
  resizeMainWindow();
  mainWindow->syncUi();
}

void Utility::toggleMenubar() {
  mainWindow->menuBar->setVisible(!mainWindow->menuBar->isVisibleTo(mainWindow));
  resizeMainWindow();
  mainWindow->shrink();
}

void Utility::toggleStatusbar() {
  mainWindow->statusBar->setVisible(!mainWindow->statusBar->isVisibleTo(mainWindow));
  resizeMainWindow();
  mainWindow->shrink();
}
