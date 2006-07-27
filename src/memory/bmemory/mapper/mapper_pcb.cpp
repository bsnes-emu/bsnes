//SHVC-1A3B-01
//SHVC-1A3B-11
//SHVC-1A3B-12
//SHVC-1A3B-13
//
//$[00-1f]:[8000-ffff] ROM P0
//$[70-7f]:[0000-ffff] RAM
//$[80-9f]:[8000-ffff] ROM P0
//$[f0-ff]:[0000-ffff] RAM
mapper(shvc_1a3b_13) {
  map(0x00, 0x1f, 0x80, 0xff, MAP_ROM);
  map(0x70, 0x7f, 0x00, 0xff, MAP_RAM);
  map(0x80, 0x9f, 0x80, 0xff, MAP_ROM);
  map(0xf0, 0xff, 0x00, 0xff, MAP_RAM);
}

//SHVC-1A3B-20
//
//$[00-7f]:[8000-ffff] ROM P0
//$[70-7f]:[0000-7fff] RAM
//$[80-ff]:[8000-ffff] ROM P0
//$[f0-ff]:[0000-7fff] RAM
mapper(shvc_1a3b_20) {
  map(0x00, 0x7f, 0x80, 0xff, MAP_ROM);
  map(0x70, 0x7f, 0x00, 0x7f, MAP_RAM);
  map(0x80, 0xff, 0x80, 0xff, MAP_ROM);
  map(0xf0, 0xff, 0x00, 0x7f, MAP_RAM);
}

//SHVC-1A3M-10
//SHVC-1A3M-20
//SHVC-1A3M-21
//SHVC-1A3M-30
//
//$[00-7f]:[8000-ffff]
//$[70-7f]:[0000-7fff]
//$[80-ff]:[8000-ffff]
//$[f0-ff]:[0000-7fff]
mapper(shvc_1a3m_30) {
  map(0x00, 0x7f, 0x80, 0xff, MAP_ROM);
  map(0x70, 0x7f, 0x00, 0x7f, MAP_RAM);
  map(0x80, 0xff, 0x80, 0xff, MAP_ROM);
  map(0xf0, 0xff, 0x00, 0x7f, MAP_RAM);
}

//unverified
//BSC-1A5M-01
//
//$[00-1f]:[8000-ffff] ROM P0
//$[20-3f]:[8000-ffff] ROM P1
//$[70-7f]:[0000-7fff] RAM
//$[80-9f]:[8000-ffff] ROM P2
//$[a0-bf]:[8000-ffff] ROM P1
//$[c0-ef]:[0000-ffff] BSX
mapper(bsc_1a5m_01) {
  map(0x00, 0x1f, 0x80, 0xff, MAP_ROM, 0x000000);
  map(0x20, 0x3f, 0x80, 0xff, MAP_ROM, 0x100000);
  map(0x70, 0x7f, 0x00, 0x7f, MAP_RAM);
  map(0x80, 0x9f, 0x80, 0xff, MAP_ROM, 0x200000);
  map(0xa0, 0xbf, 0x80, 0xff, MAP_ROM, 0x100000);
}

//unverified
//BSC-1A7M-01
//
//$[00-1f]:[8000-ffff] ROM P0
//$[20-3f]:[8000-ffff] ROM P1
//$[70-7f]:[0000-7fff] RAM
//$[80-9f]:[8000-ffff] ROM P0
//$[a0-bf]:[8000-ffff] ROM P1
//$[c0-ef]:[0000-ffff] BSX
mapper(bsc_1a7m_01) {
  map(0x00, 0x1f, 0x80, 0xff, MAP_ROM, 0x000000);
  map(0x20, 0x3f, 0x80, 0xff, MAP_ROM, 0x100000);
  map(0x70, 0x7f, 0x00, 0x7f, MAP_RAM);
  map(0x80, 0x9f, 0x80, 0xff, MAP_ROM, 0x000000);
  map(0xa0, 0xbf, 0x80, 0xff, MAP_ROM, 0x100000);
}

//unverified
//BSC-1A7M-10
//
//$[00-1f]:[8000-ffff] ROM P0
//$[20-3f]:[8000-ffff] ROM P1
//$[70-7f]:[0000-7fff] RAM
//$[80-9f]:[8000-ffff] ROM P2
//$[a0-bf]:[8000-ffff] ROM P1
//$[c0-ef]:[0000-ffff] BSX
mapper(bsc_1a7m_10) {
  map(0x00, 0x1f, 0x80, 0xff, MAP_ROM, 0x000000);
  map(0x20, 0x3f, 0x80, 0xff, MAP_ROM, 0x100000);
  map(0x70, 0x7f, 0x00, 0x7f, MAP_RAM);
  map(0x80, 0x9f, 0x80, 0xff, MAP_ROM, 0x200000);
  map(0xa0, 0xbf, 0x80, 0xff, MAP_ROM, 0x100000);
}
