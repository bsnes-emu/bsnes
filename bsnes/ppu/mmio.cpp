#include "../base.h"
#include "../timing/timing.h"
#include "../cpu/g65816.h"
extern g65816 *gx816;
extern snes_timer *snes_time;
extern videostate render;

ppustate ppu;

#include "ppu_cache.cpp"

#include "ppu_dma.cpp"
#include "ppu_screen.cpp"
#include "ppu_vram.cpp"
#include "ppu_palette.cpp"
#include "ppu_timing.cpp"
#include "ppu_oam.cpp"
#include "ppu_wram.cpp"
#include "ppu_mode7.cpp"
#include "ppu_scroll.cpp"
#include "ppu_muldiv.cpp"
#include "ppu_window.cpp"
#include "ppu_addsub.cpp"
#include "ppu_joypad.cpp"
#include "ppu_misc.cpp"

#include "ppu.cpp"

byte mmio_read(word addr) {
static word counter = 0;
  switch(addr) {
  case 0x2134:return mmio_r2134();
  case 0x2135:return mmio_r2135();
  case 0x2136:return mmio_r2136();
  case 0x2137:return mmio_r2137();
  case 0x2138:return mmio_r2138();
  case 0x2139:return mmio_r2139();
  case 0x213a:return mmio_r213a();
  case 0x213b:return mmio_r213b();
  case 0x213c:return mmio_r213c();
  case 0x213d:return mmio_r213d();
  case 0x213e:return mmio_r213e();
  case 0x213f:return mmio_r213f();

  case 0x2140:case 0x2141:case 0x2142:case 0x2143:
  case 0x2144:case 0x2145:case 0x2146:case 0x2147:
  case 0x2148:case 0x2149:case 0x214a:case 0x214b:
  case 0x214c:case 0x214d:case 0x214e:case 0x214f:
  case 0x2150:case 0x2151:case 0x2152:case 0x2153:
  case 0x2154:case 0x2155:case 0x2156:case 0x2157:
  case 0x2158:case 0x2159:case 0x215a:case 0x215b:
  case 0x215c:case 0x215d:case 0x215e:case 0x215f:
  case 0x2160:case 0x2161:case 0x2162:case 0x2163:
  case 0x2164:case 0x2165:case 0x2166:case 0x2167:
  case 0x2168:case 0x2169:case 0x216a:case 0x216b:
  case 0x216c:case 0x216d:case 0x216e:case 0x216f:
  case 0x2170:case 0x2171:case 0x2172:case 0x2173:
  case 0x2174:case 0x2175:case 0x2176:case 0x2177:
  case 0x2178:case 0x2179:case 0x217a:case 0x217b:
  case 0x217c:case 0x217d:case 0x217e:case 0x217f:
    byte x;
    x = rand() & 3;
    if(x == 0)return gx816->regs.a.b;
    if(x == 1)return gx816->regs.x;
    if(x == 2)return gx816->regs.y;
    if(x == 3) { counter++; return counter >> 1; }
    break;

  case 0x21c2:return mmio_r21c2();
  case 0x21c3:return mmio_r21c3();

  case 0x4016:return mmio_r4016();
  case 0x4210:return mmio_r4210();
  case 0x4211:return mmio_r4211();
  case 0x4212:return mmio_r4212();
  case 0x4214:return mmio_r4214();
  case 0x4215:return mmio_r4215();
  case 0x4216:return mmio_r4216();
  case 0x4217:return mmio_r4217();
  case 0x4218:return mmio_r4218();
  case 0x4219:return mmio_r4219();
  default:
/*
    dprintf("* mmio: unknown read [%0.4x]", addr);
    debug_set_state(DEBUGMODE_WAIT);
    disas_g65816_op();
*/
    break;
  }
  return 0x00;
}

void mmio_write(word addr, byte value) {
  switch(addr) {
  case 0x2100:mmio_w2100(value);break;
  case 0x2101:mmio_w2101(value);break;
  case 0x2102:mmio_w2102(value);break;
  case 0x2103:mmio_w2103(value);break;
  case 0x2104:mmio_w2104(value);break;
  case 0x2105:mmio_w2105(value);break;
  case 0x2106:mmio_w2106(value);break;
  case 0x2107:mmio_w2107(value);break;
  case 0x2108:mmio_w2108(value);break;
  case 0x2109:mmio_w2109(value);break;
  case 0x210a:mmio_w210a(value);break;
  case 0x210b:mmio_w210b(value);break;
  case 0x210c:mmio_w210c(value);break;
  case 0x210d:mmio_w210d(value);break;
  case 0x210e:mmio_w210e(value);break;
  case 0x210f:mmio_w210f(value);break;
  case 0x2110:mmio_w2110(value);break;
  case 0x2111:mmio_w2111(value);break;
  case 0x2112:mmio_w2112(value);break;
  case 0x2113:mmio_w2113(value);break;
  case 0x2114:mmio_w2114(value);break;
  case 0x2115:mmio_w2115(value);break;
  case 0x2116:mmio_w2116(value);break;
  case 0x2117:mmio_w2117(value);break;
  case 0x2118:mmio_w2118(value);break;
  case 0x2119:mmio_w2119(value);break;
  case 0x211a:mmio_w211a(value);break;
  case 0x211b:mmio_w211b(value);break;
  case 0x211c:mmio_w211c(value);break;
  case 0x211d:mmio_w211d(value);break;
  case 0x211e:mmio_w211e(value);break;
  case 0x211f:mmio_w211f(value);break;
  case 0x2120:mmio_w2120(value);break;
  case 0x2121:mmio_w2121(value);break;
  case 0x2122:mmio_w2122(value);break;
  case 0x2123:mmio_w2123(value);break;
  case 0x2124:mmio_w2124(value);break;
  case 0x2125:mmio_w2125(value);break;
  case 0x2126:mmio_w2126(value);break;
  case 0x2127:mmio_w2127(value);break;
  case 0x2128:mmio_w2128(value);break;
  case 0x2129:mmio_w2129(value);break;
  case 0x212a:mmio_w212a(value);break;
  case 0x212b:mmio_w212b(value);break;
  case 0x212c:mmio_w212c(value);break;
  case 0x212d:mmio_w212d(value);break;
  case 0x212e:mmio_w212e(value);break;
  case 0x212f:mmio_w212f(value);break;
  case 0x2130:mmio_w2130(value);break;
  case 0x2131:mmio_w2131(value);break;
  case 0x2132:mmio_w2132(value);break;
  case 0x2133:mmio_w2133(value);break;
  case 0x2180:mmio_w2180(value);break;
  case 0x2181:mmio_w2181(value);break;
  case 0x2182:mmio_w2182(value);break;
  case 0x2183:mmio_w2183(value);break;
  case 0x4016:mmio_w4016(value);break;
  case 0x4200:mmio_w4200(value);break;
  case 0x4202:mmio_w4202(value);break;
  case 0x4203:mmio_w4203(value);break;
  case 0x4204:mmio_w4204(value);break;
  case 0x4205:mmio_w4205(value);break;
  case 0x4206:mmio_w4206(value);break;
  case 0x4207:mmio_w4207(value);break;
  case 0x4208:mmio_w4208(value);break;
  case 0x4209:mmio_w4209(value);break;
  case 0x420a:mmio_w420a(value);break;
  case 0x420b:mmio_w420b(value);break;
  case 0x420c:mmio_w420c(value);break;
  case 0x420d:mmio_w420d(value);break;

  case 0x4300:case 0x4310:case 0x4320:case 0x4330:
  case 0x4340:case 0x4350:case 0x4360:case 0x4370:
    mmio_w43x0((addr >> 4) & 7, value);break;

  case 0x4301:case 0x4311:case 0x4321:case 0x4331:
  case 0x4341:case 0x4351:case 0x4361:case 0x4371:
    mmio_w43x1((addr >> 4) & 7, value);break;

  case 0x4302:case 0x4312:case 0x4322:case 0x4332:
  case 0x4342:case 0x4352:case 0x4362:case 0x4372:
    mmio_w43x2((addr >> 4) & 7, value);break;

  case 0x4303:case 0x4313:case 0x4323:case 0x4333:
  case 0x4343:case 0x4353:case 0x4363:case 0x4373:
    mmio_w43x3((addr >> 4) & 7, value);break;

  case 0x4304:case 0x4314:case 0x4324:case 0x4334:
  case 0x4344:case 0x4354:case 0x4364:case 0x4374:
    mmio_w43x4((addr >> 4) & 7, value);break;

  case 0x4305:case 0x4315:case 0x4325:case 0x4335:
  case 0x4345:case 0x4355:case 0x4365:case 0x4375:
    mmio_w43x5((addr >> 4) & 7, value);break;

  case 0x4306:case 0x4316:case 0x4326:case 0x4336:
  case 0x4346:case 0x4356:case 0x4366:case 0x4376:
    mmio_w43x6((addr >> 4) & 7, value);break;

  case 0x4307:case 0x4317:case 0x4327:case 0x4337:
  case 0x4347:case 0x4357:case 0x4367:case 0x4377:
    mmio_w43x7((addr >> 4) & 7, value);break;
  }
}
