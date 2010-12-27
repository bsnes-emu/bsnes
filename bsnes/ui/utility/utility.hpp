struct Utility : property<Utility> {
  void setTitle(const string &text);
  void updateStatus();
  void setStatus(const string &text);
  void showMessage(const string &text);

  void setControllers();
  void setScale(unsigned scale = 0);
  void setFilter();
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
