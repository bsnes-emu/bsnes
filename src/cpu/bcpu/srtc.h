void  srtc_set_time();
void  srtc_power();
void  srtc_reset();
void  srtc_write(uint8 data);
uint8 srtc_read();

#define MAX_SRTC_INDEX 0x0c

enum {
  SRTC_READ = 0,
  SRTC_WRITE,
  SRTC_COMMAND,
  SRTC_READY
};

enum {
  SRTC_COMMAND_WRITE = 0,
  SRTC_COMMAND_CLEAR = 4
};

#define DAYTICKS    (60*60*24)
#define HOURTICKS   (60*60)
#define MINUTETICKS (60)

/******************************
[srtc.data structure]
Index  Description   Range
-----  -----------   -----
    0  Seconds low   0-9
    1  Seconds high  0-5
    2  Minutes low   0-9
    3  Minutes high  0-5
    4  Hour low      0-9
    5  Hour high     0-2
    6  Day low       0-9
    7  Day high      0-3
    8  Month         1-12
    9  Year ones     0-9
   10  Year tens     0-9
   11  Year hundreds 9-11 (9=19xx, 10=20xx, 11=21xx)
   12  Day of week   0-6 (0=Sunday, ...)
******************************/

struct {
  int8   index;
  uint8  mode;
  uint8  data[MAX_SRTC_INDEX + 1];
}srtc;
