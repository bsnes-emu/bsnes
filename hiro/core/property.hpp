#if defined(Hiro_Property)
struct Property {
  using type = Property;

  Property(const string& name, const any& value = {});

  auto operator==(const Property& source) const -> bool;
  auto operator< (const Property& source) const -> bool;

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
