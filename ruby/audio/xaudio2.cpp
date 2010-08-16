/*
  audio.xaudio2 (2010-08-14)
  author: OV2
*/

#include "xaudio2.hpp"
#include <Windows.h>

namespace ruby {

class pAudioXAudio2: public IXAudio2VoiceCallback {
public:
  IXAudio2 *pXAudio2;
  IXAudio2MasteringVoice* pMasterVoice;
  IXAudio2SourceVoice *pSourceVoice;
  
  // inherited from IXAudio2VoiceCallback
  STDMETHODIMP_(void) OnBufferStart(void *pBufferContext){}
  STDMETHODIMP_(void) OnLoopEnd(void *pBufferContext){}
  STDMETHODIMP_(void) OnStreamEnd() {}
  STDMETHODIMP_(void) OnVoiceError(void *pBufferContext, HRESULT Error) {}
  STDMETHODIMP_(void) OnVoiceProcessingPassEnd() {}
  STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired) {}

  struct {
    unsigned buffers;
    unsigned latency;

    uint32_t *buffer;
    unsigned bufferoffset;

    volatile long submitbuffers;
    unsigned writebuffer;
  } device;

  struct {
    bool synchronize;
    unsigned frequency;
    unsigned latency;
  } settings;

  bool cap(const string& name) {
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    if(name == Audio::Latency) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    if(name == Audio::Latency) return settings.latency;
    return false;
  }

  bool set(const string& name, const any& value) {
    if(name == Audio::Synchronize) {
      settings.synchronize = any_cast<bool>(value);
      if(pXAudio2) clear();
      return true;
    }

    if(name == Audio::Frequency) {
      settings.frequency = any_cast<unsigned>(value);
      if(pXAudio2) init();
      return true;
    }

    if(name == Audio::Latency) {
      settings.latency = any_cast<unsigned>(value);
      if(pXAudio2) init();
      return true;
    }

    return false;
  }
  
  void pushbuffer(unsigned bytes,uint32_t *pAudioData) {
    XAUDIO2_BUFFER xa2buffer={0};
    xa2buffer.AudioBytes=bytes;
    xa2buffer.pAudioData=reinterpret_cast<BYTE *>(pAudioData);
    xa2buffer.pContext=0;
    InterlockedIncrement(&device.submitbuffers);
    pSourceVoice->SubmitSourceBuffer(&xa2buffer);
  }

  void sample(uint16_t left, uint16_t right) {
    device.buffer[device.writebuffer * device.latency + device.bufferoffset++] = left + (right << 16);
    if(device.bufferoffset < device.latency) return;
    device.bufferoffset = 0;

    if(device.submitbuffers == device.buffers - 1) {
      if(settings.synchronize == true) {
        //wait until there is at least one other free buffer for the next sample
        while(device.submitbuffers == device.buffers - 1) {
          //Sleep(0);
        }
      } else { //we need one free buffer for the next sample, so ignore the current contents
        return;
      }
    }
    
    pushbuffer(device.latency * 4,device.buffer + device.writebuffer * device.latency);

    device.writebuffer = (device.writebuffer + 1) % device.buffers;
  }

  void clear() {
    if(!pSourceVoice) return;
    pSourceVoice->Stop(0);
    pSourceVoice->FlushSourceBuffers();     //calls OnBufferEnd for all currently submitted buffers
    
    device.writebuffer = 0;

    device.bufferoffset = 0;
    if(device.buffer) memset(device.buffer, 0, device.latency * device.buffers * 4);

     pSourceVoice->Start(0);
  }

  bool init() {
    term();

    device.buffers   = 8;
    device.latency = settings.frequency * settings.latency / device.buffers / 1000.0 + 0.5;
    device.buffer  = new uint32_t[device.latency * device.buffers];
    device.bufferoffset = 0;
    device.submitbuffers = 0;

    HRESULT hr;
    if(FAILED(hr = XAudio2Create(&pXAudio2, 0 , XAUDIO2_DEFAULT_PROCESSOR))) {
      return false;
    }
    
    if(FAILED(hr = pXAudio2->CreateMasteringVoice( &pMasterVoice, 2,
        settings.frequency, 0, 0 , NULL))) {
      return false;
    }

    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = settings.frequency;
    wfx.nBlockAlign = 4;
    wfx.wBitsPerSample = 16;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    if(FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx,
        XAUDIO2_VOICE_NOSRC , XAUDIO2_DEFAULT_FREQ_RATIO, this, NULL, NULL))) {
      return false;
    }

    clear();
    return true;
  }

  void term() {
    if(pSourceVoice) {
      pSourceVoice->Stop(0);
      pSourceVoice->DestroyVoice();
      pSourceVoice = 0;
    }
    if(pMasterVoice) {
      pMasterVoice->DestroyVoice();
      pMasterVoice = 0;
    }
    if(pXAudio2) {
      pXAudio2->Release();
      pXAudio2 = NULL;
    }
    if(device.buffer) {
      delete[] device.buffer;
      device.buffer = 0;
    }
  }
  
  STDMETHODIMP_(void) OnBufferEnd(void *pBufferContext) {
    InterlockedDecrement(&device.submitbuffers);
  }

  pAudioXAudio2() {
    pXAudio2 = 0;
    pMasterVoice = 0;
    pSourceVoice = 0;

    device.buffer = 0;
    device.bufferoffset = 0;
    device.submitbuffers = 0;
    device.writebuffer = 0;

    settings.synchronize = false;
    settings.frequency = 22050;
    settings.latency = 120;
  }
};

DeclareAudio(XAudio2)

};
