struct Utility {
  void setMode(Interface::Mode mode);
  void resizeMainWindow(bool shrink = false);
  void toggleFullScreen();
  void bindVideoShader();

  void updateStatus();
  void setStatusText(const string &text);
  void showMessage(const string &text);

  Utility();

private:
  time_t statusTime;
  string statusText;
  string statusMessage;
};

extern Utility *utility;
