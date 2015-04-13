#ifdef NALL_STRING_INTERNAL_HPP

//convert any (supported) type to a const char* without constructing a new nall::string
//this is used inside string{...} to build nall::string values

namespace nall {

//base types

template<> struct stringify<bool> {
  bool _value;
  auto data() const -> const char* { return _value ? "true" : "false"; }
  auto size() const -> unsigned { return _value ? 4 : 5; }
  stringify(bool value) : _value(value) {}
};

template<> struct stringify<char> {
  char _data[2];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return 1; }
  stringify(char source) { _data[0] = source; _data[1] = 0; }
};

//signed integers

template<> struct stringify<signed char> {
  char _data[2 + sizeof(signed char) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(signed char source) { integer(_data, source); }
};

template<> struct stringify<signed short> {
  char _data[2 + sizeof(signed short) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(signed short source) { integer(_data, source); }
};

template<> struct stringify<signed int> {
  char _data[2 + sizeof(signed int) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(signed int source) { integer(_data, source); }
};

template<> struct stringify<signed long> {
  char _data[2 + sizeof(signed long) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(signed long source) { integer(_data, source); }
};

template<> struct stringify<signed long long> {
  char _data[2 + sizeof(signed long long) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(signed long long source) { integer(_data, source); }
};

template<unsigned bits> struct stringify<int_t<bits>> {
  char _data[2 + sizeof(intmax_t) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(int_t<bits> source) { integer(_data, source); }
};

//unsigned integers

template<> struct stringify<unsigned char> {
  char _data[1 + sizeof(unsigned char) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(unsigned char source) { decimal(_data, source); }
};

template<> struct stringify<unsigned short> {
  char _data[1 + sizeof(unsigned short) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(unsigned short source) { decimal(_data, source); }
};

template<> struct stringify<unsigned int> {
  char _data[1 + sizeof(unsigned int) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(unsigned int source) { decimal(_data, source); }
};

template<> struct stringify<unsigned long> {
  char _data[1 + sizeof(unsigned long) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(unsigned long source) { decimal(_data, source); }
};

template<> struct stringify<unsigned long long> {
  char _data[1 + sizeof(unsigned long long) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(unsigned long long source) { decimal(_data, source); }
};

template<unsigned bits> struct stringify<uint_t<bits>> {
  char _data[1 + sizeof(uintmax_t) * 3];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(uint_t<bits> source) { decimal(_data, source); }
};

//floating-point

template<> struct stringify<float> {
  char _data[256];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(float source) { real(_data, source); }
};

template<> struct stringify<double> {
  char _data[256];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(double source) { real(_data, source); }
};

template<> struct stringify<long double> {
  char _data[256];
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(long double source) { real(_data, source); }
};

//arrays

template<> struct stringify<vector<uint8_t>> {
  vector<char> _text;
  auto data() const -> const char* { return _text.data(); }
  auto size() const -> unsigned { return _text.size(); }
  stringify(vector<uint8_t> source) {
    _text.resize(source.size());
    memory::copy(_text.data(), source.data(), source.size());
  }
};

template<> struct stringify<const vector<uint8_t>&> {
  vector<char> _text;
  auto data() const -> const char* { return _text.data(); }
  auto size() const -> unsigned { return _text.size(); }
  stringify(const vector<uint8_t>& source) {
    _text.resize(source.size());
    memory::copy(_text.data(), source.data(), source.size());
  }
};

//char arrays

template<> struct stringify<char*> {
  const char* _data;
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(char* source) : _data(source) {}
};

template<> struct stringify<const char*> {
  const char* _data;
  auto data() const -> const char* { return _data; }
  auto size() const -> unsigned { return strlen(_data); }
  stringify(const char* source) : _data(source) {}
};

//strings

template<> struct stringify<string> {
  const string& _text;
  auto data() const -> const char* { return _text.data(); }
  auto size() const -> unsigned { return _text.size(); }
  stringify(const string& source) : _text(source) {}
};

template<> struct stringify<const string&> {
  const string& _text;
  auto data() const -> const char* { return _text.data(); }
  auto size() const -> unsigned { return _text.size(); }
  stringify(const string& source) : _text(source) {}
};

#if defined(QSTRING_H)

//Qt

template<> struct stringify<QString> {
  const QString& _text;
  auto data() const -> const char* { return _text.toUtf8().constData(); }
  auto size() const -> unsigned { return _text.size(); }
  stringify(const QString& source) : _text(source) {}
};

template<> struct stringify<const QString&> {
  const QString& _text;
  auto data() const -> const char* { return _text.toUtf8().constData(); }
  auto size() const -> unsigned { return _text.size(); }
  stringify(const QString& source) : _text(source) {}
};

string::operator QString() const {
  return QString::fromUtf8(*this);
}

#endif

//

template<typename T> auto make_string(T value) -> stringify<T> {
  return stringify<T>(std::forward<T>(value));
}

}

#endif
