namespace Heuristics {

struct Memory {
  auto& type(string type) { _type = type; return *this; }
  auto& battery(boolean battery = true) { _battery = battery; return *this; }
  auto& size(natural size) { _size = size; return *this; }
  auto& content(string content) { _content = content; return *this; }
  auto& manufacturer(string manufacturer) { _manufacturer = manufacturer; return *this; }
  auto& architecture(string architecture) { _architecture = architecture; return *this; }
  auto& identifier(string identifier) { _identifier = identifier; return *this; }
  auto text() const -> string;

  string _type;
  boolean _battery;
  natural _size;
  string _content;
  string _manufacturer;
  string _architecture;
  string _identifier;
};

struct Oscillator {
  auto& frequency(natural frequency) { _frequency = frequency; return *this; }
  auto text() const -> string;

  natural _frequency;
};

}
