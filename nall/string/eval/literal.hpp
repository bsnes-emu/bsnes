#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {
namespace Eval {

inline bool isLiteral(const char*& s) {
  char n = s[0];
  return (n >= 'A' && n <= 'Z')
      || (n >= 'a' && n <= 'z')
      || (n >= '0' && n <= '9')
      || n == '_' || n == '\"';
}

inline string literalNumber(const char*& s) {
  const char* p = s;

  //binary
  if(p[0] == '0' && p[1] == 'b') {
    p += 2;
    while(p[0] == '0' || p[0] == '1') p++;
    if(p - s < 3) throw "invalid binary literal";
    string result = substr(s, 0, p - s);
    s = p;
    return result;
  }

  //octal
  if(p[0] == '0' && p[1] == 'o') {
    p += 2;
    while(p[0] >= '0' && p[0] <= '7') p++;
    if(p - s < 3) throw "invalid octal literal";
    string result = substr(s, 0, p - s);
    s = p;
    return result;
  }

  //hex
  if(p[0] == '0' && p[1] == 'x') {
    p += 2;
    while((p[0] >= '0' && p[0] <= '9') || (p[0] >= 'A' && p[0] <= 'F') || (p[0] >= 'a' && p[0] <= 'f')) p++;
    if(p - s < 3) throw "invalid hex literal";
    string result = substr(s, 0, p - s);
    s = p;
    return result;
  }

  //decimal
  while(p[0] >= '0' && p[0] <= '9') p++;
  if(p[0] != '.') {
    string result = substr(s, 0, p - s);
    s = p;
    return result;
  }

  //floating-point
  p++;
  while(p[0] >= '0' && p[0] <= '9') p++;
  string result = substr(s, 0, p - s);
  s = p;
  return result;
}

inline string literalString(const char*& s) {
  const char* p = s + 1;

  while(p[0] && p[0] != '\"') p++;
  if(*p++ != '\"') throw "unclosed string literal";

  string result = substr(s, 0, p - s);
  s = p;
  return result;
}

inline string literalVariable(const char*& s) {
  const char* p = s;

  while(p[0] == '_' || (p[0] >= 'A' && p[0] <= 'Z') || (p[0] >= 'a' && p[0] <= 'z') || (p[0] >= '0' && p[0] <= '9')) p++;

  string result = substr(s, 0, p - s);
  s = p;
  return result;
}

inline string literal(const char*& s) {
  const char* p = s;

  if(p[0] >= '0' && p[0] <= '9') return literalNumber(s);
  if(p[0] == '\"') return literalString(s);
  if(p[0] == '_' || (p[0] >= 'A' && p[0] <= 'Z') || (p[0] >= 'a' && p[0] <= 'z')) return literalVariable(s);

  throw "invalid literal";
}

}
}

#endif
