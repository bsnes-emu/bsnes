#if defined(Hiro_Attribute)
struct Attribute {
  using type = Attribute;

  Attribute(const string& name, const any& value = {});

  auto operator==(const Attribute& source) const -> bool;
  auto operator< (const Attribute& source) const -> bool;

  auto name() const -> string;
  auto setValue(const any& value = {}) -> type&;
  auto value() const -> any&;

private:
  struct State {
    string name;
    mutable any value;
  } state;
};
#endif
