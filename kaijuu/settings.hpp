struct Settings {
  struct Rule {
    string name;
    string pattern;
    bool defaultAction;
    bool matchFiles;
    bool matchFolders;
    string command;
    bool multiSelection;
  };
  vector<Rule> rules;

  void load() {
    rules.reset();
    lstring ruleIDs = registry::contents("HKCU/Software/Kaijuu/");
    for(auto &ruleID : ruleIDs) {
      string path = {"HKCU/Software/Kaijuu/", ruleID, "/"};
      rules.append({
        registry::read({path, "Name"}),
        registry::read({path, "Pattern"}),
        registry::read({path, "Default"}) == "true",
        registry::read({path, "Files"}) == "true",
        registry::read({path, "Folders"}) == "true",
        registry::read({path, "Command"}),
      });
    }
    for(auto &rule : rules) {
      rule.multiSelection = rule.command.position("{paths}") || rule.command.position("{files}");
    }
  }

  void save() {
    registry::remove("HKCU/Software/Kaijuu/");
    for(unsigned id = 0; id < rules.size(); id++) {
      auto &rule = rules(id);
      string path = {"HKCU/Software/Kaijuu/", id, "/"};
      registry::write({path, "Name"}, rule.name);
      registry::write({path, "Pattern"}, rule.pattern);
      registry::write({path, "Default"}, rule.defaultAction);
      registry::write({path, "Files"}, rule.matchFiles);
      registry::write({path, "Folders"}, rule.matchFolders);
      registry::write({path, "Command"}, rule.command);
    }
  }
} settings;
