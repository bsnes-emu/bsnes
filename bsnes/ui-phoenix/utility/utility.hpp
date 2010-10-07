struct Utility : property<Utility> {
  void setTitle(const char *text);
  void updateStatus();
  void setStatus(const char *text);
  void showMessage(const char *text);

  void setControllers();
  void setScale(unsigned scale = 0);
  void setShader();

  void cartridgeLoaded();
  void cartridgeUnloaded();

  void saveState(unsigned slot);
  void loadState(unsigned slot);

  Utility();

private:
  string statusCurrentText;
  string statusText;
  string statusMessage;
  time_t statusTime;
};

extern Utility utility;
