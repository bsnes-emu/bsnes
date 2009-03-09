class Status {
public:
  void update();
  void flush();
  bool enqueue(const char *message);

  Status();

private:
  Window &window;
  string text;

  struct Queue {
    bool valid;
    string text;
  } queue[16];
  time_t displaytime;
} status;
