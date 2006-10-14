/*
  c-style Math Evaluator v1.0 (07/03/2006)
  Algorithm: Recursive Descent Processor
  Written by: gladius
  Optimized by: gladius and byuu
  Public Domain
*/

#define maxlevel 12
int strmath_rdp(const char *&s, int level = 0) {
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
    } else if(*s >= '0' && *s <= '9') {
    int num, len;
      sscanf(s, "%d%n", &num, &len);
      s += len;
      return num;
    }
  }

int lhs = strmath_rdp(s, level + 1);
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

int strmath(const char *s) {
  return strmath_rdp(s);
}

#ifdef __LIBSTRING
int strmath(string &s) {
  return strmath(strptr(s));
}
#endif
