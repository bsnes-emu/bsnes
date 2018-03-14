namespace Heuristics {

struct Memory {
  auto& type(string type) { _type = type; return *this; }
  auto& battery(boolean battery = true) { _battery = battery; return *this; }
  auto& size(natural size) { _size = size; return *this; }
  auto& category(string category) { _category = category; return *this; }
  auto& manufacturer(string manufacturer) { _manufacturer = manufacturer; return *this; }
  auto& model(string model) { _model = model; return *this; }
  auto& identity(string identity) { _identity = identity; return *this; }
  auto text() const -> string;

  string _type;
  boolean _battery;
  natural _size;
  string _category;
  string _manufacturer;
  string _model;
  string _identity;
};

struct Oscillator {
  auto& frequency(natural frequency) { _frequency = frequency; return *this; }
  auto text() const -> string;

  natural _frequency;
};

}
