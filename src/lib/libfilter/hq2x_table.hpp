/*****
 * HQ2x Algorithm (C) 2003 Maxim Stepin
 * License: LGPL
 *
 * Optimizations (C) 2006 Shay Green, byuu
 *****/

#define p00(color) *(out0 + 0) = (colortable[color])
#define p01(color) *(out0 + 1) = (colortable[color])
#define p10(color) *(out1 + 0) = (colortable[color])
#define p11(color) *(out1 + 1) = (colortable[color])

#define PIXEL00_0     p00(        W5);
#define PIXEL00_10    p00(blend1 (W5, W1));
#define PIXEL00_11    p00(blend1 (W5, W4));
#define PIXEL00_12    p00(blend1 (W5, W2));
#define PIXEL00_20    p00(blend2 (W5, W4, W2));
#define PIXEL00_21    p00(blend2 (W5, W1, W2));
#define PIXEL00_22    p00(blend2 (W5, W1, W4));
#define PIXEL00_60    p00(blend6 (W5, W2, W4));
#define PIXEL00_61    p00(blend6 (W5, W4, W2));
#define PIXEL00_70    p00(blend7 (W5, W4, W2));
#define PIXEL00_90    p00(blend9 (W5, W4, W2));
#define PIXEL00_100   p00(blend10(W5, W4, W2));

#define PIXEL01_0     p01(        W5);
#define PIXEL01_10    p01(blend1 (W5, W3));
#define PIXEL01_11    p01(blend1 (W5, W2));
#define PIXEL01_12    p01(blend1 (W5, W6));
#define PIXEL01_20    p01(blend2 (W5, W2, W6));
#define PIXEL01_21    p01(blend2 (W5, W3, W6));
#define PIXEL01_22    p01(blend2 (W5, W3, W2));
#define PIXEL01_60    p01(blend6 (W5, W6, W2));
#define PIXEL01_61    p01(blend6 (W5, W2, W6));
#define PIXEL01_70    p01(blend7 (W5, W2, W6));
#define PIXEL01_90    p01(blend9 (W5, W2, W6));
#define PIXEL01_100   p01(blend10(W5, W2, W6));

#define PIXEL10_0     p10(        W5);
#define PIXEL10_10    p10(blend1 (W5, W7));
#define PIXEL10_11    p10(blend1 (W5, W8));
#define PIXEL10_12    p10(blend1 (W5, W4));
#define PIXEL10_20    p10(blend2 (W5, W8, W4));
#define PIXEL10_21    p10(blend2 (W5, W7, W4));
#define PIXEL10_22    p10(blend2 (W5, W7, W8));
#define PIXEL10_60    p10(blend6 (W5, W4, W8));
#define PIXEL10_61    p10(blend6 (W5, W8, W4));
#define PIXEL10_70    p10(blend7 (W5, W8, W4));
#define PIXEL10_90    p10(blend9 (W5, W8, W4));
#define PIXEL10_100   p10(blend10(W5, W8, W4));

#define PIXEL11_0     p11(        W5);
#define PIXEL11_10    p11(blend1 (W5, W9));
#define PIXEL11_11    p11(blend1 (W5, W6));
#define PIXEL11_12    p11(blend1 (W5, W8));
#define PIXEL11_20    p11(blend2 (W5, W6, W8));
#define PIXEL11_21    p11(blend2 (W5, W9, W8));
#define PIXEL11_22    p11(blend2 (W5, W9, W6));
#define PIXEL11_60    p11(blend6 (W5, W8, W6));
#define PIXEL11_61    p11(blend6 (W5, W6, W8));
#define PIXEL11_70    p11(blend7 (W5, W6, W8));
#define PIXEL11_90    p11(blend9 (W5, W6, W8));
#define PIXEL11_100   p11(blend10(W5, W6, W8));

  case 0:
  case 1:
  case 4:
  case 32:
  case 128:
  case 5:
  case 132:
  case 160:
  case 33:
  case 129:
  case 36:
  case 133:
  case 164:
  case 161:
  case 37:
  case 165: {
    PIXEL00_20
    PIXEL01_20
    PIXEL10_20
    PIXEL11_20
  } break;

  case 2:
  case 34:
  case 130:
  case 162: {
    PIXEL00_22
    PIXEL01_21
    PIXEL10_20
    PIXEL11_20
  } break;

  case 16:
  case 17:
  case 48:
  case 49: {
    PIXEL00_20
    PIXEL01_22
    PIXEL10_20
    PIXEL11_21
  } break;

  case 64:
  case 65:
  case 68:
  case 69: {
    PIXEL00_20
    PIXEL01_20
    PIXEL10_21
    PIXEL11_22
  } break;

  case 8:
  case 12:
  case 136:
  case 140: {
    PIXEL00_21
    PIXEL01_20
    PIXEL10_22
    PIXEL11_20
  } break;

  case 3:
  case 35:
  case 131:
  case 163: {
    PIXEL00_11
    PIXEL01_21
    PIXEL10_20
    PIXEL11_20
  } break;

  case 6:
  case 38:
  case 134:
  case 166: {
    PIXEL00_22
    PIXEL01_12
    PIXEL10_20
    PIXEL11_20
  } break;

  case 20:
  case 21:
  case 52:
  case 53: {
    PIXEL00_20
    PIXEL01_11
    PIXEL10_20
    PIXEL11_21
  } break;

  case 144:
  case 145:
  case 176:
  case 177: {
    PIXEL00_20
    PIXEL01_22
    PIXEL10_20
    PIXEL11_12
  } break;

  case 192:
  case 193:
  case 196:
  case 197: {
    PIXEL00_20
    PIXEL01_20
    PIXEL10_21
    PIXEL11_11
  } break;

  case 96:
  case 97:
  case 100:
  case 101: {
    PIXEL00_20
    PIXEL01_20
    PIXEL10_12
    PIXEL11_22
  } break;

  case 40:
  case 44:
  case 168:
  case 172: {
    PIXEL00_21
    PIXEL01_20
    PIXEL10_11
    PIXEL11_20
  } break;

  case 9:
  case 13:
  case 137:
  case 141: {
    PIXEL00_12
    PIXEL01_20
    PIXEL10_22
    PIXEL11_20
  } break;

        case 18:
        case 50:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_20
          PIXEL11_21
          break;
        }
        case 80:
        case 81:
        {
          PIXEL00_20
          PIXEL01_22
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 72:
        case 76:
        {
          PIXEL00_21
          PIXEL01_20
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_22
          break;
        }
        case 10:
        case 138:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_21
          PIXEL10_22
          PIXEL11_20
          break;
        }
        case 66:
        {
          PIXEL00_22
          PIXEL01_21
          PIXEL10_21
          PIXEL11_22
          break;
        }
        case 24:
        {
          PIXEL00_21
          PIXEL01_22
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 7:
        case 39:
        case 135:
        {
          PIXEL00_11
          PIXEL01_12
          PIXEL10_20
          PIXEL11_20
          break;
        }
        case 148:
        case 149:
        case 180:
        {
          PIXEL00_20
          PIXEL01_11
          PIXEL10_20
          PIXEL11_12
          break;
        }
        case 224:
        case 228:
        case 225:
        {
          PIXEL00_20
          PIXEL01_20
          PIXEL10_12
          PIXEL11_11
          break;
        }
        case 41:
        case 169:
        case 45:
        {
          PIXEL00_12
          PIXEL01_20
          PIXEL10_11
          PIXEL11_20
          break;
        }
        case 22:
        case 54:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_20
          PIXEL11_21
          break;
        }
        case 208:
        case 209:
        {
          PIXEL00_20
          PIXEL01_22
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 104:
        case 108:
        {
          PIXEL00_21
          PIXEL01_20
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_22
          break;
        }
        case 11:
        case 139:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_21
          PIXEL10_22
          PIXEL11_20
          break;
        }
        case 19:
        case 51:
        {
          if (diff(W2, W6))
          {
            PIXEL00_11
            PIXEL01_10
          }
          else
          {
            PIXEL00_60
            PIXEL01_90
          }
          PIXEL10_20
          PIXEL11_21
          break;
        }
        case 146:
        case 178:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_10
            PIXEL11_12
          }
          else
          {
            PIXEL01_90
            PIXEL11_61
          }
          PIXEL10_20
          break;
        }
        case 84:
        case 85:
        {
          PIXEL00_20
          if (diff(W6, W8))
          {
            PIXEL01_11
            PIXEL11_10
          }
          else
          {
            PIXEL01_60
            PIXEL11_90
          }
          PIXEL10_21
          break;
        }
        case 112:
        case 113:
        {
          PIXEL00_20
          PIXEL01_22
          if (diff(W6, W8))
          {
            PIXEL10_12
            PIXEL11_10
          }
          else
          {
            PIXEL10_61
            PIXEL11_90
          }
          break;
        }
        case 200:
        case 204:
        {
          PIXEL00_21
          PIXEL01_20
          if (diff(W8, W4))
          {
            PIXEL10_10
            PIXEL11_11
          }
          else
          {
            PIXEL10_90
            PIXEL11_60
          }
          break;
        }
        case 73:
        case 77:
        {
          if (diff(W8, W4))
          {
            PIXEL00_12
            PIXEL10_10
          }
          else
          {
            PIXEL00_61
            PIXEL10_90
          }
          PIXEL01_20
          PIXEL11_22
          break;
        }
        case 42:
        case 170:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
            PIXEL10_11
          }
          else
          {
            PIXEL00_90
            PIXEL10_60
          }
          PIXEL01_21
          PIXEL11_20
          break;
        }
        case 14:
        case 142:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
            PIXEL01_12
          }
          else
          {
            PIXEL00_90
            PIXEL01_61
          }
          PIXEL10_22
          PIXEL11_20
          break;
        }
        case 67:
        {
          PIXEL00_11
          PIXEL01_21
          PIXEL10_21
          PIXEL11_22
          break;
        }
        case 70:
        {
          PIXEL00_22
          PIXEL01_12
          PIXEL10_21
          PIXEL11_22
          break;
        }
        case 28:
        {
          PIXEL00_21
          PIXEL01_11
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 152:
        {
          PIXEL00_21
          PIXEL01_22
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 194:
        {
          PIXEL00_22
          PIXEL01_21
          PIXEL10_21
          PIXEL11_11
          break;
        }
        case 98:
        {
          PIXEL00_22
          PIXEL01_21
          PIXEL10_12
          PIXEL11_22
          break;
        }
        case 56:
        {
          PIXEL00_21
          PIXEL01_22
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 25:
        {
          PIXEL00_12
          PIXEL01_22
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 26:
        case 31:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 82:
        case 214:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 88:
        case 248:
        {
          PIXEL00_21
          PIXEL01_22
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 74:
        case 107:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_21
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_22
          break;
        }
        case 27:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_10
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 86:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_21
          PIXEL11_10
          break;
        }
        case 216:
        {
          PIXEL00_21
          PIXEL01_22
          PIXEL10_10
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 106:
        {
          PIXEL00_10
          PIXEL01_21
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_22
          break;
        }
        case 30:
        {
          PIXEL00_10
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 210:
        {
          PIXEL00_22
          PIXEL01_10
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 120:
        {
          PIXEL00_21
          PIXEL01_22
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_10
          break;
        }
        case 75:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_21
          PIXEL10_10
          PIXEL11_22
          break;
        }
        case 29:
        {
          PIXEL00_12
          PIXEL01_11
          PIXEL10_22
          PIXEL11_21
          break;
        }
        case 198:
        {
          PIXEL00_22
          PIXEL01_12
          PIXEL10_21
          PIXEL11_11
          break;
        }
        case 184:
        {
          PIXEL00_21
          PIXEL01_22
          PIXEL10_11
          PIXEL11_12
          break;
        }
        case 99:
        {
          PIXEL00_11
          PIXEL01_21
          PIXEL10_12
          PIXEL11_22
          break;
        }
        case 57:
        {
          PIXEL00_12
          PIXEL01_22
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 71:
        {
          PIXEL00_11
          PIXEL01_12
          PIXEL10_21
          PIXEL11_22
          break;
        }
        case 156:
        {
          PIXEL00_21
          PIXEL01_11
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 226:
        {
          PIXEL00_22
          PIXEL01_21
          PIXEL10_12
          PIXEL11_11
          break;
        }
        case 60:
        {
          PIXEL00_21
          PIXEL01_11
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 195:
        {
          PIXEL00_11
          PIXEL01_21
          PIXEL10_21
          PIXEL11_11
          break;
        }
        case 102:
        {
          PIXEL00_22
          PIXEL01_12
          PIXEL10_12
          PIXEL11_22
          break;
        }
        case 153:
        {
          PIXEL00_12
          PIXEL01_22
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 58:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 83:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 92:
        {
          PIXEL00_21
          PIXEL01_11
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 202:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          PIXEL01_21
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          PIXEL11_11
          break;
        }
        case 78:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          PIXEL11_22
          break;
        }
        case 154:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 114:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 89:
        {
          PIXEL00_12
          PIXEL01_22
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 90:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 55:
        case 23:
        {
          if (diff(W2, W6))
          {
            PIXEL00_11
            PIXEL01_0
          }
          else
          {
            PIXEL00_60
            PIXEL01_90
          }
          PIXEL10_20
          PIXEL11_21
          break;
        }
        case 182:
        case 150:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_0
            PIXEL11_12
          }
          else
          {
            PIXEL01_90
            PIXEL11_61
          }
          PIXEL10_20
          break;
        }
        case 213:
        case 212:
        {
          PIXEL00_20
          if (diff(W6, W8))
          {
            PIXEL01_11
            PIXEL11_0
          }
          else
          {
            PIXEL01_60
            PIXEL11_90
          }
          PIXEL10_21
          break;
        }
        case 241:
        case 240:
        {
          PIXEL00_20
          PIXEL01_22
          if (diff(W6, W8))
          {
            PIXEL10_12
            PIXEL11_0
          }
          else
          {
            PIXEL10_61
            PIXEL11_90
          }
          break;
        }
        case 236:
        case 232:
        {
          PIXEL00_21
          PIXEL01_20
          if (diff(W8, W4))
          {
            PIXEL10_0
            PIXEL11_11
          }
          else
          {
            PIXEL10_90
            PIXEL11_60
          }
          break;
        }
        case 109:
        case 105:
        {
          if (diff(W8, W4))
          {
            PIXEL00_12
            PIXEL10_0
          }
          else
          {
            PIXEL00_61
            PIXEL10_90
          }
          PIXEL01_20
          PIXEL11_22
          break;
        }
        case 171:
        case 43:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
            PIXEL10_11
          }
          else
          {
            PIXEL00_90
            PIXEL10_60
          }
          PIXEL01_21
          PIXEL11_20
          break;
        }
        case 143:
        case 15:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
            PIXEL01_12
          }
          else
          {
            PIXEL00_90
            PIXEL01_61
          }
          PIXEL10_22
          PIXEL11_20
          break;
        }
        case 124:
        {
          PIXEL00_21
          PIXEL01_11
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_10
          break;
        }
        case 203:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_21
          PIXEL10_10
          PIXEL11_11
          break;
        }
        case 62:
        {
          PIXEL00_10
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 211:
        {
          PIXEL00_11
          PIXEL01_10
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 118:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_12
          PIXEL11_10
          break;
        }
        case 217:
        {
          PIXEL00_12
          PIXEL01_22
          PIXEL10_10
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 110:
        {
          PIXEL00_10
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_22
          break;
        }
        case 155:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_10
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 188:
        {
          PIXEL00_21
          PIXEL01_11
          PIXEL10_11
          PIXEL11_12
          break;
        }
        case 185:
        {
          PIXEL00_12
          PIXEL01_22
          PIXEL10_11
          PIXEL11_12
          break;
        }
        case 61:
        {
          PIXEL00_12
          PIXEL01_11
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 157:
        {
          PIXEL00_12
          PIXEL01_11
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 103:
        {
          PIXEL00_11
          PIXEL01_12
          PIXEL10_12
          PIXEL11_22
          break;
        }
        case 227:
        {
          PIXEL00_11
          PIXEL01_21
          PIXEL10_12
          PIXEL11_11
          break;
        }
        case 230:
        {
          PIXEL00_22
          PIXEL01_12
          PIXEL10_12
          PIXEL11_11
          break;
        }
        case 199:
        {
          PIXEL00_11
          PIXEL01_12
          PIXEL10_21
          PIXEL11_11
          break;
        }
        case 220:
        {
          PIXEL00_21
          PIXEL01_11
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 158:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 234:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          PIXEL01_21
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_11
          break;
        }
        case 242:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 59:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 121:
        {
          PIXEL00_12
          PIXEL01_22
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 87:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 79:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          PIXEL11_22
          break;
        }
        case 122:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 94:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 218:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 91:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 229:
        {
          PIXEL00_20
          PIXEL01_20
          PIXEL10_12
          PIXEL11_11
          break;
        }
        case 167:
        {
          PIXEL00_11
          PIXEL01_12
          PIXEL10_20
          PIXEL11_20
          break;
        }
        case 173:
        {
          PIXEL00_12
          PIXEL01_20
          PIXEL10_11
          PIXEL11_20
          break;
        }
        case 181:
        {
          PIXEL00_20
          PIXEL01_11
          PIXEL10_20
          PIXEL11_12
          break;
        }
        case 186:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_11
          PIXEL11_12
          break;
        }
        case 115:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 93:
        {
          PIXEL00_12
          PIXEL01_11
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 206:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          PIXEL11_11
          break;
        }
        case 205:
        case 201:
        {
          PIXEL00_12
          PIXEL01_20
          if (diff(W8, W4))
          {
            PIXEL10_10
          }
          else
          {
            PIXEL10_70
          }
          PIXEL11_11
          break;
        }
        case 174:
        case 46:
        {
          if (diff(W4, W2))
          {
            PIXEL00_10
          }
          else
          {
            PIXEL00_70
          }
          PIXEL01_12
          PIXEL10_11
          PIXEL11_20
          break;
        }
        case 179:
        case 147:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_10
          }
          else
          {
            PIXEL01_70
          }
          PIXEL10_20
          PIXEL11_12
          break;
        }
        case 117:
        case 116:
        {
          PIXEL00_20
          PIXEL01_11
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_10
          }
          else
          {
            PIXEL11_70
          }
          break;
        }
        case 189:
        {
          PIXEL00_12
          PIXEL01_11
          PIXEL10_11
          PIXEL11_12
          break;
        }
        case 231:
        {
          PIXEL00_11
          PIXEL01_12
          PIXEL10_12
          PIXEL11_11
          break;
        }
        case 126:
        {
          PIXEL00_10
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_10
          break;
        }
        case 219:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_10
          PIXEL10_10
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 125:
        {
          if (diff(W8, W4))
          {
            PIXEL00_12
            PIXEL10_0
          }
          else
          {
            PIXEL00_61
            PIXEL10_90
          }
          PIXEL01_11
          PIXEL11_10
          break;
        }
        case 221:
        {
          PIXEL00_12
          if (diff(W6, W8))
          {
            PIXEL01_11
            PIXEL11_0
          }
          else
          {
            PIXEL01_60
            PIXEL11_90
          }
          PIXEL10_10
          break;
        }
        case 207:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
            PIXEL01_12
          }
          else
          {
            PIXEL00_90
            PIXEL01_61
          }
          PIXEL10_10
          PIXEL11_11
          break;
        }
        case 238:
        {
          PIXEL00_10
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_0
            PIXEL11_11
          }
          else
          {
            PIXEL10_90
            PIXEL11_60
          }
          break;
        }
        case 190:
        {
          PIXEL00_10
          if (diff(W2, W6))
          {
            PIXEL01_0
            PIXEL11_12
          }
          else
          {
            PIXEL01_90
            PIXEL11_61
          }
          PIXEL10_11
          break;
        }
        case 187:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
            PIXEL10_11
          }
          else
          {
            PIXEL00_90
            PIXEL10_60
          }
          PIXEL01_10
          PIXEL11_12
          break;
        }
        case 243:
        {
          PIXEL00_11
          PIXEL01_10
          if (diff(W6, W8))
          {
            PIXEL10_12
            PIXEL11_0
          }
          else
          {
            PIXEL10_61
            PIXEL11_90
          }
          break;
        }
        case 119:
        {
          if (diff(W2, W6))
          {
            PIXEL00_11
            PIXEL01_0
          }
          else
          {
            PIXEL00_60
            PIXEL01_90
          }
          PIXEL10_12
          PIXEL11_10
          break;
        }
        case 237:
        case 233:
        {
          PIXEL00_12
          PIXEL01_20
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          PIXEL11_11
          break;
        }
        case 175:
        case 47:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          PIXEL01_12
          PIXEL10_11
          PIXEL11_20
          break;
        }
        case 183:
        case 151:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          PIXEL10_20
          PIXEL11_12
          break;
        }
        case 245:
        case 244:
        {
          PIXEL00_20
          PIXEL01_11
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
        case 250:
        {
          PIXEL00_10
          PIXEL01_10
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 123:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_10
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_10
          break;
        }
        case 95:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_10
          PIXEL11_10
          break;
        }
        case 222:
        {
          PIXEL00_10
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_10
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 252:
        {
          PIXEL00_21
          PIXEL01_11
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
        case 249:
        {
          PIXEL00_12
          PIXEL01_22
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 235:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_21
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          PIXEL11_11
          break;
        }
        case 111:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_22
          break;
        }
        case 63:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_11
          PIXEL11_21
          break;
        }
        case 159:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          PIXEL10_22
          PIXEL11_12
          break;
        }
        case 215:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          PIXEL10_21
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 246:
        {
          PIXEL00_22
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
        case 254:
        {
          PIXEL00_10
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
        case 253:
        {
          PIXEL00_12
          PIXEL01_11
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
        case 251:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          PIXEL01_10
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 239:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          PIXEL01_12
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          PIXEL11_11
          break;
        }
        case 127:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_20
          }
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_20
          }
          PIXEL11_10
          break;
        }
        case 191:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          PIXEL10_11
          PIXEL11_12
          break;
        }
        case 223:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_20
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          PIXEL10_10
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_20
          }
          break;
        }
        case 247:
        {
          PIXEL00_11
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          PIXEL10_12
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
        case 255:
        {
          if (diff(W4, W2))
          {
            PIXEL00_0
          }
          else
          {
            PIXEL00_100
          }
          if (diff(W2, W6))
          {
            PIXEL01_0
          }
          else
          {
            PIXEL01_100
          }
          if (diff(W8, W4))
          {
            PIXEL10_0
          }
          else
          {
            PIXEL10_100
          }
          if (diff(W6, W8))
          {
            PIXEL11_0
          }
          else
          {
            PIXEL11_100
          }
          break;
        }
