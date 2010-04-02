class sBus : public Bus {
public:
  bool load_cart();
  void unload_cart();

  void power();
  void reset();

  void serialize(serializer&);
  sBus();
  ~sBus();

private:
  void map_reset();
  void map_xml();
  void map_system();
};

extern sBus bus;
