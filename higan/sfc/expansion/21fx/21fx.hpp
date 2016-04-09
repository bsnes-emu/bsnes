struct S21FX : Cothread, Memory {
  static auto Enter() -> void;
  auto main() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

private:
  auto quit() -> bool;
  auto usleep(uint) -> void;
  auto readable() -> bool;
  auto writable() -> bool;
  auto read() -> uint8;
  auto write(uint8) -> void;

  bool booted = false;
  uint16 resetVector;
  uint8 ram[122];

  nall::library link;
  function<void (
    function<bool ()>,      //quit
    function<void (uint)>,  //usleep
    function<bool ()>,      //readable
    function<bool ()>,      //writable
    function<uint8 ()>,     //read
    function<void (uint8)>  //write
  )> linkInit;
  function<void (lstring)> linkMain;

  vector<uint8> snesBuffer;  //SNES -> Link
  vector<uint8> linkBuffer;  //Link -> SNES
};

extern S21FX s21fx;
