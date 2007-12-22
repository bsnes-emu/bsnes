#ifdef BSTRING_H

/*
  recursive descent math processor v0.02 (2007-11-28)
  authors: gladius, byuu
  license: public domain
*/

void strmath_space(const char *&s) {
  while(*s == ' ' || *s == '\t')s++;
}

int strmath_number(const char *&s) {
int result = 0;
enum type { dec, hex, bin, oct };
type t = dec;
  if(*s == '0') {
    s++;
    if(*s == 'x' || *s == 'X') { s++; t = hex; }
    else if(*s == 'b' || *s == 'B') { s++; t = bin; }
    else { t = oct; }
  } else if(*s == '$') {
    s++;
    t = hex;
  } else if(*s == '%') {
    s++;
    t = bin;
  }

  switch(t) {

  case dec: {
    while(true) {
    char x = *s;
      if(x >= '0' && x <= '9')x = x - '0';
      else break;
      result = result * 10 + x;
      s++;
    }
  } break;

  case hex: {
    while(true) {
    char x = *s;
      if(x >= '0' && x <= '9')x = x - '0';
      else if(x >= 'a' && x <= 'f')x = x - 'a' + 0x0a;
      else if(x >= 'A' && x <= 'F')x = x - 'A' + 0x0a;
      else break;
      result = result * 16 + x;
      s++;
    }
  } break;

  case bin: {
    while(true) {
    char x = *s;
      if(x >= '0' && x <= '1')x = x - '0';
      else break;
      result = result * 2 + x;
      s++;
    }
  } break;

  case oct: {
    while(true) {
    char x = *s;
      if(x >= '0' && x <= '7')x = x - '0';
      else break;
      result = result * 8 + x;
      s++;
    }
  } break;

  }

  return result;

int num, len;
  sscanf(s, "%d%n", &num, &len);
  s += len;
  return num;
}

#define maxlevel 12
int strmath_rdp(const char *&s, int level = 0) {
  strmath_space(s);

  if(level == maxlevel) {
    if(*s == '(') {
    int result = strmath_rdp(++s, 0);
      s++;
      return result;
    } else if(*s == '+') {
      return +strmath_rdp(++s, maxlevel);
    } else if(*s == '-') {
      return -strmath_rdp(++s, maxlevel);
    } else if(*s == '!') {
      return !strmath_rdp(++s, maxlevel);
    } else if(*s == '~') {
      return ~strmath_rdp(++s, maxlevel);
    } else if((*s >= '0' && *s <= '9') || *s == '$' || *s == '%') {
      return strmath_number(s);
    } else {
      return 0; //error
    }
  }

int lhs = strmath_rdp(s, level + 1);
  strmath_space(s);
  while(*s) {
  int a = *s;
  int b = *s ? *(s + 1) : 0;
    switch(level) {
    case 0:
      if(a == '?')break;
      return lhs;
    case 1:
      if(a == '|' && b == '|') { s++; break; }
      return lhs;
    case 2:
      if(a == '^' && b == '^') { s++; break; }
      return lhs;
    case 3:
      if(a == '&' && b == '&') { s++; break; }
      return lhs;
    case 4:
      if(a == '|' && b != '|')break;
      return lhs;
    case 5:
      if(a == '^' && b != '^')break;
      return lhs;
    case 6:
      if(a == '&' && b != '&')break;
      return lhs;
    case 7:
      if(a == '=' && b == '=') { s++; break; }
      if(a == '!' && b == '=') { s++; break; }
      return lhs;
    case 8:
      if(a == '<' && b == '=') { s++; break; }
      if(a == '>' && b == '=') { s++; break; }
      if(a == '<')break;
      if(a == '>')break;
      return lhs;
    case 9:
      if(a == '<' && b == '<') { s++; break; }
      if(a == '>' && b == '>') { s++; break; }
      return lhs;
    case 10:
      if(a == '+')break;
      if(a == '-')break;
      return lhs;
    case 11:
      if(a == '*')break;
      if(a == '/')break;
      if(a == '%')break;
      return lhs;
    }
    if(a == '?') {
    int tr = strmath_rdp(++s, level);
    int fr = strmath_rdp(++s, level);
      lhs = (lhs) ? tr : fr;
    } else {
    int rhs = strmath_rdp(++s, level + 1);
      if     (a == '|' && b == '|') lhs = (lhs || rhs);
      else if(a == '^' && b == '^') lhs = (!lhs != !rhs);
      else if(a == '&' && b == '&') lhs = (lhs && rhs);
      else if(a == '|' && b != '|') lhs |= rhs;
      else if(a == '^' && b != '^') lhs ^= rhs;
      else if(a == '&' && b != '&') lhs &= rhs;
      else if(a == '=' && b == '=') lhs = (lhs == rhs);
      else if(a == '!' && b == '=') lhs = (lhs != rhs);
      else if(a == '<' && b == '=') lhs = (lhs <= rhs);
      else if(a == '>' && b == '=') lhs = (lhs >= rhs);
      else if(a == '<' && b != '<') lhs = (lhs < rhs);
      else if(a == '>' && b != '>') lhs = (lhs > rhs);
      else if(a == '<' && b == '<') lhs <<= rhs;
      else if(a == '>' && b == '>') lhs >>= rhs;
      else if(a == '+')             lhs += rhs;
      else if(a == '-')             lhs -= rhs;
      else if(a == '*')             lhs *= rhs;
      else if(a == '/')             lhs /= rhs;
      else if(a == '%')             lhs %= rhs;
    }
  }
  return lhs;
}
#undef maxlevel

int strmath(const char *str) {
  return strmath_rdp(str);
}

#endif //ifdef BSTRING_H
