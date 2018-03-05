namespace Heuristics {

struct Memory {
  auto& type(string type) { _type = type; return *this; }
  auto& size(natural size) { _size = size; return *this; }
  auto& category(string category) { _category = category; return *this; }
  auto& manufacturer(string manufacturer) { _manufacturer = manufacturer; return *this; }
  auto& part(string part) { _part = part; return *this; }
  auto& battery(boolean battery = true) { _battery = battery; return *this; }
  auto& note(string note) { _note = note; return *this; }
  auto text() const -> string;

  string _type;
  natural _size;
  string _category;
  string _manufacturer;
  string _part;
  boolean _battery;
  string _note;
};

struct Oscillator {
  auto& frequency(natural frequency) { _frequency = frequency; return *this; }
  auto& note(string note) { _note = note; return *this; }
  auto text() const -> string;

  natural _frequency;
  string _note;
};

struct Heuristics {
  auto memory(string type, uint size, string name, string metadata = {}) const -> string;
};

}
