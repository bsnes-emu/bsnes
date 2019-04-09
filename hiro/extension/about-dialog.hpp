#if defined(Hiro_AboutDialog)

struct AboutDialog {
  using type = AboutDialog;

  auto setAlignment(Alignment = Alignment::Center) -> type&;
  auto setAlignment(sWindow relativeTo, Alignment = Alignment::Center) -> type&;
  auto setAuthor(const string& author = "") -> type&;
  auto setDescription(const string& description = "") -> type&;
  auto setLicense(const string& license = "") -> type&;
  auto setLogo(const image& logo = {}) -> type&;
  auto setName(const string& name = "") -> type&;
  auto setVersion(const string& version = "") -> type&;
  auto setWebsite(const string& website = "") -> type&;
  auto show() -> void;

private:
  struct State {
    Alignment alignment = Alignment::Center;
    string author;
    string description;
    string license;
    image logo;
    string name;
    sWindow relativeTo;
    string version;
    string website;
  } state;
};

#endif
