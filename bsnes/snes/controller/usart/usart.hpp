struct USART : Controller, public library {
  uint2 data();
  void latch(bool data);
  USART(bool port);
  ~USART();

private:
  bool latched;
  bool data1;
  bool data2;

  uint8 rxlength;
  uint8 rxdata;

  uint8 txlength;
  uint8 txdata;
};
