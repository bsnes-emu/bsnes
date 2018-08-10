#if defined(Hiro_Group)
struct mGroup : mObject {
  Declare(Group)
  using mObject::remove;

  auto append(sObject object) -> type&;
  auto object(uint offset) const -> Object;
  auto objectCount() const -> uint;
  auto objects() const -> vector<Object>;
  auto remove(sObject object) -> type&;

//private:
  struct State {
    vector<wObject> objects;
  } state;
};
#endif
