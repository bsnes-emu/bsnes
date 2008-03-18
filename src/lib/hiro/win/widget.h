class pWidget {
public:
  Widget &self;
  virtual void show(bool = true);
  virtual void hide();
  virtual bool visible();
  virtual uintptr_t handle();

  pWidget(Widget&);
  virtual ~pWidget();

  /* internal */

  //Windows API controls often require a unique ID for each control to identify it.
  //Simulate this with an instance counter, so that each Widget has a unique ID.
  //In each pWidget() constructor, instance = instance_counter++; is called.
  static uint instance_counter;
  uint instance;
};
