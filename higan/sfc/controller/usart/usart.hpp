struct USART : Controller, public library {
  USART(bool port);
  ~USART();

  auto enter() -> void;

  auto quit() -> bool;
  auto usleep(unsigned milliseconds) -> void;
  auto readable() -> bool;
  auto read() -> uint8;
  auto writable() -> bool;
  auto write(uint8 data) -> void;

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  bool data1;
  bool data2;
  unsigned counter;

  uint8 rxlength;
  uint8 rxdata;
  vector<uint8> rxbuffer;

  uint8 txlength;
  uint8 txdata;
  vector<uint8> txbuffer;

  function<void (
    function<bool ()>,          //quit
    function<void (unsigned)>,  //usleep
    function<bool ()>,          //readable
    function<uint8 ()>,         //read
    function<bool ()>,          //writable
    function<void (uint8)>      //write
  )> init;
  function<void ()> main;
};
