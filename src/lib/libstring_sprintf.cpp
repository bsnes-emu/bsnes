void numtobin(char *s, uint32 num) {
uint32 mask = 0x80000000, len = 0, z = 0;
  for(;mask;mask>>=1,len++) { if(num&mask)break; }
  len = 32 - len;
  do {
    if(num&(1<<(len-1)))s[z++] = '1';
    else                s[z++] = '0';
  }while(--len);
  s[z] = 0;
}

void sprintf(_string &str, char *s, ...) {
va_list args;
char t[2], n[256];
int i, l, sl, z;
uint8 pad_type, pad_len;
uint32 num;
char *r;
  va_start(args, s);
  strcpy(str, "");
  for(i=0;i<strlen(s);i++) {
    if(s[i] == '%') {
      i++;
      if(s[i] == '0' && s[i+1] == '.' && (s[i+2] >= '0' && s[i+2] <= '9')) {
        pad_type = 1;
        if(s[i+3] >= '0' && s[i+3] <= '9') { pad_len = (s[i+2]-'0')*10 + (s[i+3]-'0'); i+=4; }
        else                               { pad_len = (s[i+2]-'0');                   i+=3; }
      }
      else if(s[i] >= '0' && s[i] <= '9') {
        pad_type = 2;
        if(s[i+1] >= '0' && s[i+1] <= '9') { pad_len = (s[i]-'0')*10 + (s[i+1]-'0'); i+=2; }
        else                               { pad_len = (s[i]-'0');                   i+=1; }
      }
      else { pad_type = 0; }

      if(s[i] == 'd') {
        num = va_arg(args, uint32);
        sprintf(n, "%d", num);
      } else if(s[i] == 'x') {
        num = va_arg(args, uint32);
        sprintf(n, "%x", num);
      } else if(s[i] == 'b') {
        num = va_arg(args, uint32);
        numtobin(n, num);
      } else if(s[i] == 's') {
        r = va_arg(args, char*);
      }

      if(pad_type != 0) {
        if(s[i] == 's')sl = strlen(r);
        else           sl = strlen(n);
        if(sl < pad_len) {
          while(sl < pad_len) {
            strcat(str, (pad_type == 1)?"0":" ");
            sl++;
          }
        }
      }

      if(s[i] == 's')strcat(str, r);
      else           strcat(str, n);
    } else {
      t[0] = s[i];
      t[1] = 0;
      strcat(str, t);
    }
  }
  va_end(args);
}
