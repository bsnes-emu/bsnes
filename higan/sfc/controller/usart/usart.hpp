struct USART : Controller, public library {
  USART(bool port);
  ~USART();

  auto main() -> void;

  auto quit() -> bool;
  auto usleep(uint microseconds) -> void;
  auto readable() -> bool;
  auto read() -> uint8;
  auto writable() -> bool;
  auto write(uint8 data) -> void;

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched = 0;
  bool data1 = 0;
  bool data2 = 0;
  uint counter = 0;

  uint8 rxlength = 0;
  uint8 rxdata = 0;
  vector<uint8> rxbuffer;

  uint8 txlength = 0;
  uint8 txdata = 0;
  vector<uint8> txbuffer;

  function<void (
    function<bool ()>,      //quit
    function<void (uint)>,  //usleep
    function<bool ()>,      //readable
    function<uint8 ()>,     //read
    function<bool ()>,      //writable
    function<void (uint8)>  //write
  )> usart_init;
  function<void (lstring)> usart_main;
};
