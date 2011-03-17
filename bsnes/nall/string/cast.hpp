#ifndef NALL_STRING_CAST_HPP
#define NALL_STRING_CAST_HPP

namespace nall {

//this is needed, as C++0x does not support explicit template specialization inside classes
template<> inline const char* to_string<bool>         (bool v)          { return v ? "true" : "false"; }
template<> inline const char* to_string<signed int>   (signed int v)    { static char temp[256]; snprintf(temp, 255, "%+d", v); return temp; }
template<> inline const char* to_string<unsigned int> (unsigned int v)  { static char temp[256]; snprintf(temp, 255, "%u", v); return temp; }
template<> inline const char* to_string<double>       (double v)        { static char temp[256]; snprintf(temp, 255, "%f", v); return temp; }
template<> inline const char* to_string<char*>        (char *v)         { return v; }
template<> inline const char* to_string<const char*>  (const char *v)   { return v; }
template<> inline const char* to_string<string>       (string v)        { return v; }
template<> inline const char* to_string<const string&>(const string &v) { return v; }

template<typename T> string& string::operator= (T value) { return assign(to_string<T>(value)); }
template<typename T> string& string::operator<<(T value) { return append(to_string<T>(value)); }

template<typename T> lstring& lstring::operator<<(T value) {
  operator[](size()).assign(to_string<T>(value));
  return *this;
}

#if defined(QSTRING_H)
template<> inline const char* to_string<QString>(QString v) { return v.toUtf8().constData(); }
template<> inline const char* to_string<const QString&>(const QString &v) { return v.toUtf8().constData(); }
string::operator QString() const { return QString::fromUtf8(*this); }
#endif

}

#endif
