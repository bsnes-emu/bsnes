#define COBJMACROS
#include "audio.h"
#include <Windows.h>
#ifdef REDIST_XAUDIO
#include <xaudio2redist.h>
#else
#include <xaudio2.h>
#endif
#include <initguid.h>
#include <Mmdeviceapi.h>

// This is a hack, but Windows itself is a hack so I don't care
#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
DEFINE_GUID(CLSID_##className, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)

#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
DEFINE_GUID(IID_##interfaceName, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)

DEFINE_CLSID(MMDeviceEnumerator, bcde0395, e52f, 467c, 8e, 3d, c4, 57, 92, 91, 69, 2e);
DEFINE_IID(IMMDeviceEnumerator,  a95664d2, 9614, 4f35, a7, 46, de, 8d, b6, 36, 17, e6);


static unsigned audio_frequency = 48000;
static IXAudio2 *xaudio2 = NULL;
static IXAudio2MasteringVoice *master_voice = NULL;
static IXAudio2SourceVoice *source_voice = NULL;
static bool playing = false;
static GB_sample_t sample_pool[0x2000];
static unsigned pos = 0;

#define BATCH_SIZE 256

static WAVEFORMATEX wave_format = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 2,
    .nBlockAlign = 4,
    .wBitsPerSample = 16,
    .cbSize = 0
};

static bool _audio_is_playing(void)
{
    return playing;
}

static void _audio_clear_queue(void)
{
    pos = 0;
    IXAudio2SourceVoice_FlushSourceBuffers(source_voice);
}

static void _audio_set_paused(bool paused)
{
    if (paused) {
        playing = false;
        IXAudio2SourceVoice_Stop(source_voice, 0, XAUDIO2_COMMIT_NOW);
        _audio_clear_queue();
    }
    else {
        playing = true;
        IXAudio2SourceVoice_Start(source_voice, 0, XAUDIO2_COMMIT_NOW);
    }
    
}


#define _DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) static const PROPERTYKEY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }
_DEFINE_PROPERTYKEY(_PKEY_AudioEngine_DeviceFormat, 0xf19f064d, 0x82c, 0x4e27, 0xbc, 0x73, 0x68, 0x82, 0xa1, 0xbb, 0x8e, 0x4c, 0);


static void update_frequency(void)
{
    HRESULT hr;
    IMMDevice  *device = NULL;
    IMMDeviceEnumerator *enumerator = NULL;
    IPropertyStore *store = NULL;
    PWAVEFORMATEX deviceFormatProperties;
    PROPVARIANT prop;
    
    hr = CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, &IID_IMMDeviceEnumerator, (LPVOID *)&enumerator);
    if (FAILED(hr)) return;
    
    // get default audio endpoint
    
    hr = IMMDeviceEnumerator_GetDefaultAudioEndpoint(enumerator, eRender, eMultimedia, &device);
    if (FAILED(hr)) return;
    
    hr = IMMDevice_OpenPropertyStore(device, STGM_READ, &store);
    if (FAILED(hr)) return;
    
    hr = IPropertyStore_GetValue(store, &_PKEY_AudioEngine_DeviceFormat, &prop);
    if (FAILED(hr)) return;
    
    deviceFormatProperties = (PWAVEFORMATEX)prop.blob.pBlobData;
    audio_frequency = deviceFormatProperties->nSamplesPerSec;
    if (audio_frequency < 8000 || audio_frequency > 192000) {
        // Bogus value, revert to 48KHz
        audio_frequency = 48000;
    }
}

static unsigned _audio_get_frequency(void)
{
    return audio_frequency;
}

static size_t _audio_get_queue_length(void)
{
    static XAUDIO2_VOICE_STATE state;
    IXAudio2SourceVoice_GetState(source_voice, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    
    return state.BuffersQueued * BATCH_SIZE + (pos & (BATCH_SIZE - 1));
}

static void _audio_queue_sample(GB_sample_t *sample)
{
    if (!playing) return;
        
    static XAUDIO2_BUFFER buffer = {.AudioBytes = sizeof(*sample) * BATCH_SIZE, };
    sample_pool[pos] = *sample;
    buffer.pAudioData = (void *)&sample_pool[pos & ~(BATCH_SIZE - 1)];
    pos++;
    pos &= 0x1fff;
    if ((pos & (BATCH_SIZE - 1)) == 0) {
        IXAudio2SourceVoice_SubmitSourceBuffer(source_voice, &buffer, NULL);
    }
}

static bool _audio_init(void)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        fprintf(stderr, "CoInitializeEx failed: %lx\n", hr);
        return false;
    }
    
    hr = XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        fprintf(stderr, "XAudio2Create failed: %lx\n", hr);
        return false;
    }
    
    update_frequency();
    
    hr = IXAudio2_CreateMasteringVoice(xaudio2, &master_voice,
                                       2, // 2 channels
                                       audio_frequency,
                                       0, // Flags
                                       0, // Device index
                                       NULL, // Effect chain
                                       AudioCategory_GameMedia // Category
                                      );
    if (FAILED(hr)) {
        fprintf(stderr, "CreateMasteringVoice failed: %lx\n", hr);
        return false;
    }
    
    wave_format.nSamplesPerSec = audio_frequency;
    wave_format.nAvgBytesPerSec = audio_frequency * 4;
    hr = IXAudio2_CreateSourceVoice(xaudio2, &source_voice, &wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    
    if (FAILED(hr)) {
        fprintf(stderr, "CreateSourceVoice failed: %lx\n", hr);
        return false;
    }
    
    return true;
}

static void _audio_deinit(void)
{
    _audio_set_paused(true);
}

GB_AUDIO_DRIVER(XAudio2);
