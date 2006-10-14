uint sprintf(string &str, const char *s, ...) {
va_list args;
  va_start(args, s);
uint length = vsprintf(str, s, args);
  va_end(args);
  return length;
}

uint vsprintf(string &str, const char *s, va_list args) {
uint length = vsnprintf(0, 0, s, args);
  strresize(str, length + 1);
  return vsprintf(strptr(str), s, args);
}

/*
uint vsprintf(string &str, const char *s, va_list args) {
bool   leftalign;
bool   showbase;
char   positivesign;
char   pad;
uint   width;
bool   useprecision;
uint   precision;
char   modifier;
char   type;

uint32 vararg32;
uint64 vararg64;
char  *varptr;
char   varstr[256];
uint   varstrlen;

  strcpy(str, "");
uint i = 0;
  while(s[i]) {
    if(s[i] != '%') {
      strcat(str, s[i++]);
    } else if(s[i + 1] == '%' || !s[i + 1]) {
      strcat(str, '%');
    } else { //s[i] == '%'
    //format: %[flags][width][.precision][modifiers]type
      i++;

    //flags
      leftalign    = false;
      positivesign = 0;
      showbase     = false;

      while(s[i]) {
        if(s[i] == '-') {
          leftalign = true;
          i++;
        } else if(s[i] == '+') {
          positivesign = '+';
          i++;
        } else if(s[i] == ' ') {
          positivesign = ' ';
          i++;
        } else if(s[i] == '#') {
          showbase = true;
          i++;
        } else {
          break;
        }
      }

    //zero padding
      if(s[i] == '0') {
        pad = '0';
        i++;
      } else {
        pad = ' ';
      }

    //width
      width = 0;
      if(s[i] == '*') {
        width = va_arg(args, uint32);
        i++;
      } else {
        while(s[i]) {
          if(s[i] < '0' || s[i] > '9')break;
          width *= 10;
          width += s[i++] - '0';
        }
      }
      if(width == 0)width = 1;

    //precision
      useprecision = false;
      precision    = 0;
      if(s[i] == '.') {
        useprecision = true;
        i++;

        if(s[i] == '*') {
          precision = va_arg(args, uint32);
          i++;
        } else {
          while(s[i]) {
            if(s[i] < '0' || s[i] > '9')break;
            precision *= 10;
            precision += s[i++] - '0';
          }
        }
      }
      if(precision == 0)precision = 1;

    //modifier
      if(s[i] == 'h') {
        modifier = 'h';
        i++;
      } else if(s[i] == 'l') {
        modifier = 'l';
        i++;
      } else if(s[i] == 'L') {
        modifier = 'L';
        i++;
      }

    //type
      type = s[i++];
      switch(type) {

      case 'c': {
      //character
        vararg32 = va_arg(args, uint32);

        if(leftalign == false) { while(width-- > 1)strcat(str, pad); }
        strcat(str, char(vararg32));
        if(leftalign ==  true) { while(width-- > 1)strcat(str, pad); }
      } break;

      case 's': {
      //string
        varptr = va_arg(args, char*);

        varstrlen = strlen(varptr);
        if(useprecision && precision < varstrlen)varstrlen = precision;
        if(leftalign == false) { while(width-- > varstrlen)strcat(str, pad); }
      uint index = 0;
      //todo: optimize to a fixed-width strcat
        while(index < varstrlen)strcat(str, varptr[index++]);
        if(leftalign ==  true) { while(width-- > varstrlen)strcat(str, pad); }
      } break;

      case 'd':
      case 'i':
      case 'u':
      case 'x':
      case 'X': {
      //signed integer
        vararg32 = va_arg(args, uint32);

        if(type == 'd' || type == 'i') {
          itoa(varstr, vararg32);
        } else if(type == 'u') {
          utoa(varstr, vararg32);
        } else if(type == 'x') {
          htoa(varstr, vararg32);
        } else if(type == 'X') {
          uhtoa(varstr, vararg32);
        }

      uint basestrlen = strlen(varstr);
        varstrlen = (useprecision && precision > basestrlen) ? precision : basestrlen;

        if(type == 'd' || type == 'i') {
          if(int32(vararg32) >= 0 && positivesign) { varstrlen++; }
        }

        if(type == 'x' || type == 'X') {
          if(showbase) { varstrlen += 2; }
        }

        if(leftalign == false) { while(width-- > varstrlen)strcat(str, pad); }
        if(type == 'd' || type == 'i') {
          if(int32(vararg32) >= 0 && positivesign) { strcat(str, positivesign); }
        }
        if(type == 'x' || type == 'X') {
          if(showbase)strcat(str, (type == 'x') ? "0x" : "0X");
        }
        if(useprecision) {
          while(basestrlen < precision) { strcat(str, "0"); basestrlen++; }
        }
        strcat(str, varstr);
        if(leftalign ==  true) { while(width-- > varstrlen)strcat(str, pad); }
      } break;

      case 'p': {
      //todo: add 64-bit pointer support
        vararg32 = va_arg(args, uint32);

        strcpy(varstr, "00000000");
      uint index = 8;
        htoa(varstr + index, vararg32);
        varstrlen = strlen(varstr + index);
        index -= 8 - varstrlen;

        if(leftalign == false) { while(width-- > 8)strcat(str, pad); }
        strcat(str, varstr + index);
        if(leftalign ==  true) { while(width-- > 8)strcat(str, pad); }
      } break;

      }
    }
  }
  return strlen(str);
}
*/
