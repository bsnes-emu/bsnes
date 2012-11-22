struct Configuration : configuration {
  string path;

  Configuration() {
    append(path = userpath(), "Path");
    directory::create({configpath(), "ananke/"});
    load({configpath(), "ananke/settings.cfg"});
  }

  ~Configuration() {
    save({configpath(), "ananke/settings.cfg"});
  }
} config;
