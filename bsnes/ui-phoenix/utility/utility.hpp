struct Utility {
  void setTitle(const char *text);
  void updateStatus();
  void setStatus(const char *text);
  void showMessage(const char *text);

  void loadCartridgeNormal();
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
