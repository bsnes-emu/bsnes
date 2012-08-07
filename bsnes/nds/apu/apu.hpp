
struct APU {
  void power();
  
  void stepMixer();
  
  void stepVoice(unsigned no);
  void stepPCM8(unsigned no);
  void stepPCM16(unsigned no);
  void stepADPCM4(unsigned no);
  void stepPulse(unsigned no);
  void stepNoise(unsigned no);
  
  void stopVoice(unsigned no);
  void fillBuffer(unsigned no);
  void checkLength(unsigned no);
  void checkEnable(unsigned no);
  
  
  uint32 regControl();
  uint32 regBias();
  uint32 regCaptureControl();
  uint32 regCaptureDest(unsigned no);
  uint32 regVoiceControl(unsigned no);
  
  void regControl(uint32 data, uint32 mask);
  void regBias(uint32 data, uint32 mask);
  void regCaptureControl(uint32 data, uint32 mask);
  void regCaptureDest(unsigned no, uint32 data, uint32 mask);
  void regCaptureLength(unsigned no, uint32 data, uint32 mask);
  
  void regVoiceControl(unsigned no, uint32 data, uint32 mask);
  void regVoiceSource(unsigned no, uint32 data, uint32 mask);
  void regVoicePeriod(unsigned no, uint32 data, uint32 mask);
  void regVoiceLength(unsigned no, uint32 data, uint32 mask);
  
  
  
  struct Voice {
    uint1 enable, hold, running;
    uint2 format;  enum { PCM8, PCM16, ADPCM4, PSG };
    uint3 duty;
    uint2 limit;   enum { looped=1, once=2 };
    uint7 panning;
    
    // Volume is essentially some kind of float format.
    // The effective value is base * 2^-(4 >> (3-exponent)).
    uint7 volumeBase;
    uint2 volumeExp;
    
    struct {
      uint32 source;
      uint16 counter;
      uint16 length1;
      uint32 length2;
    } init;
    
    struct {
      uint32 source;
      uint32 length;
      int16  sample;
      int16  state;
    } loop;
    
    uint32 source;
    uint32 length;
    uint16 counter;
    
    Event event;
    
    uint5  index;      // nibble index into 16-byte buffer
    uint32 buffer[4];  // holds 8, 16, or 32 buffered samples
    int16  state;      // used for pulse, noise, and ADPCM
    
    int32  amplitude;
    int16  sample;
  } voices[16];
  
  Event mixEvent;
  
  // Voices 0 + 2 (L,R) are designated here as streaming audio.
  // Voices 1 + 3 (L,R) are meant for DSP effects. Both are optional though.
  uint1  powered;
  uint1  enable;
  uint10 bias;
  uint7  volume;
  uint2  output[2];  enum { srcMixer, srcDspL, srcDspR, srcDspMono };
  uint1  muteDsp[2];         // Don't send 1+3 to the mixer - avoids feedback.
  uint1  dspToStream[3];     // Mix 1+3 back into 0+2.
  
  // Capture enables writing mixed audio to RAM - and in conjunction with
  // voices 1 + 3, allows software DSP such as filtering or echo effects.
  uint1  capture[2];         // Enable capturing
  uint1  captureStream[2];   // ..otherwise mixer outputs
  uint32 captureDest[2];
  uint32 captureLength[2];
  uint32 captureCount[2];
};

extern APU apu;