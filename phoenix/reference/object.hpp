namespace phoenix {

struct pObject {
  Object& object;
  bool locked;

  pObject(Object& object) : object(object), locked(locked) {}
  virtual ~pObject() {}

  void constructor();
  void destructor();
};

}
