#define STRMATH_ADD  1
#define STRMATH_SUB  2
#define STRMATH_MUL  3
#define STRMATH_DIV  4
#define STRMATH_MOD  5
#define STRMATH_AND  6
#define STRMATH_OR   7
#define STRMATH_XOR  8
#define STRMATH_SHL  9
#define STRMATH_SHR 10

#define STRMATH_LINKED 64

#define STRMATHMODE_NEG 1
#define STRMATHMODE_NOT 2

#define __strunktonum()                                         \
  if     (s1[0] == '0' && s1[1] == 'x')r = strhex(s1 + 2); \
  else if(s1[0] == '0' && s1[1] == 'b')r = strbin(s1 + 2); \
  else r = strdec(s1)

#define __strmath_setmode()                       \
  if     (str[i] == '-') { mode = STRMATHMODE_NEG; i++; } \
  else if(str[i] == '~') { mode = STRMATHMODE_NOT; i++; } \
  else if(str[i] == '+') {                         i++; } \
  else mode=0

#define __strmath_modeset()            \
  if     (mode == STRMATHMODE_NEG)r *= -1; \
  else if(mode == STRMATHMODE_NOT)r =~ r

#define __strmath_set(__x)     \
  s1[z] = 0;                   \
  z = 0;                       \
  __strunktonum();             \
  __strmath_modeset();         \
  array[array_size++] = r;     \
  array_gate[array_size] = __x

/***************************************
strmath(str)
  resolves all math entities from within
  str, and returns numerical result
  example: strmath("5+5")=10
***************************************/
uint32 p_strmath(char *str) {
int i = 0, ssl = strlen(str);
uint8 x, mode = 0;
uint32 r, array[128], array_size = 0, z = 0;
uint8 array_gate[128];
char *s1;
  if(!ssl)return 0;
  s1 = (char*)malloc(ssl + 1);
  __strmath_setmode();
  while(i < ssl) {
    x = str[i++];
    if     (x == '+') { __strmath_set(STRMATH_ADD); __strmath_setmode(); }
    else if(x == '-') { __strmath_set(STRMATH_SUB); __strmath_setmode(); }
    else if(x == '*') { __strmath_set(STRMATH_MUL); __strmath_setmode(); }
    else if(x == '/') { __strmath_set(STRMATH_DIV); __strmath_setmode(); }
    else if(x == '%') { __strmath_set(STRMATH_MOD); __strmath_setmode(); }
    else if(x == '&') { __strmath_set(STRMATH_AND); __strmath_setmode(); }
    else if(x == '|') { __strmath_set(STRMATH_OR ); __strmath_setmode(); }
    else if(x == '^') { __strmath_set(STRMATH_XOR); __strmath_setmode(); }
    else if(x == '<' && str[i] == '<') { __strmath_set(STRMATH_SHL); i++; __strmath_setmode(); }
    else if(x == '>' && str[i] == '>') { __strmath_set(STRMATH_SHR); i++; __strmath_setmode(); }
    else s1[z++] = x;
  }
  s1[z] = 0;
  __strunktonum();
  __strmath_modeset();
  array[array_size++] = r;
  free(s1);

  for(i=1;i<array_size;i++) {
    if     (array_gate[i] == STRMATH_SHL) { array[i-1] <<= array[i]; array_gate[i] = STRMATH_LINKED; }
    else if(array_gate[i] == STRMATH_SHR) { array[i-1] >>= array[i]; array_gate[i] = STRMATH_LINKED; }
  }

  for(i=1;i<array_size;i++) {
    if     (array_gate[i] == STRMATH_MUL) { array[i-1] *= array[i]; array_gate[i] = STRMATH_LINKED; }
    else if(array_gate[i] == STRMATH_DIV) { array[i-1] /= array[i]; array_gate[i] = STRMATH_LINKED; }
  }

  r = array[0];
  for(i=1;i<array_size;i++) {
    if     (array_gate[i] == STRMATH_ADD)r += array[i];
    else if(array_gate[i] == STRMATH_SUB)r -= array[i];
    else if(array_gate[i] == STRMATH_MOD)r %= array[i];
    else if(array_gate[i] == STRMATH_AND)r &= array[i];
    else if(array_gate[i] == STRMATH_OR )r |= array[i];
    else if(array_gate[i] == STRMATH_XOR)r ^= array[i];
  }

  return r;
}

uint32 strmath(char *in_str) {
uint32 r = 0;
uint32 pdepth = 0, cpdepth, maxpdepth = 0;
uint32 pstart, pend, spos;
int i, sc, sl = strlen(in_str);
char *str = (char*)malloc(sl + 1), *str0;
char *pstr;
char num[64];
  strcpy(str, in_str);

  for(i=0;i<sl;i++) {
    if(str[i]=='(') {
      pdepth++;
      if(pdepth > maxpdepth)maxpdepth = pdepth;
    } else if(str[i]==')') {
      if(pdepth == 0) {
        free(str);
        return null; //error! too many )'s
      }
      pdepth --;
    }
  }

  if(pdepth != 0) {
    free(str);
    return null; //error! unequal ('s to )'s
  }

  pdepth = maxpdepth;
  while(pdepth) {
    cpdepth = 0;
    for(i=0;i<sl;) {
      if(str[i] == '(')cpdepth++;
      if(str[i] == ')')cpdepth--;
      i++;
      if(cpdepth == pdepth) {
        pstart = i;
        while(str[i] != ')')i++;
        pend = i;

        pstr = (char*)malloc(pend-pstart+1);
        memcpy(pstr, str+pstart, pend-pstart);
        pstr[pend-pstart]=0;
        r = p_strmath(pstr);
        free(pstr);
        sprintf(num, "%d", r);
        str0 = (char*)malloc(sl + strlen(num) + 1);
        memcpy(str0, str, pstart - 1);
        spos = pstart - 1;
        memcpy(str0+spos, num, strlen(num));
        spos += strlen(num);
        sc = spos;
        memcpy(str0+spos, str+pend+1, sl-pend-1);
        spos += sl - pend - 1;
        sl = spos;
        str0[sl] = 0;
        free(str);
        str = str0;
        cpdepth--;
        i = sc;
      }
    }
    pdepth--;
  }

  r = p_strmath(str);

  free(str);
  return r;
}

uint32 strmathentity(char *str) {
int i, ssl = strlen(str);
  for(i=0;i<ssl;i++) {
    if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' ||
       str[i] == '%' || str[i] == '&' || str[i] == '|' || str[i] == '^' ||
      (str[i] == '<' && str[i+1] == '<') || (str[i] == '>' && str[i+1] == '>'))return 1;
  }
  return 0;
}
