#define COBJMACROS
#include "xaudio2_7.h"
#include "audio.h"
#include <Mmdeviceapi.h>


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

static inline HRESULT XAudio2Create(IXAudio2 **out,
                                        UINT32 Flags,
                                        XAUDIO2_PROCESSOR XAudio2Processor)
{
    IXAudio2 *xaudio2;
    LoadLibraryEx("xaudio2_7.dll", NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    
    HRESULT hr = CoCreateInstance(&CLSID_XAudio2, NULL, CLSCTX_INPROC_SERVER, &IID_IXAudio2, (void **)&xaudio2);
    if (SUCCEEDED(hr)) {
        hr = xaudio2->lpVtbl->Initialize(xaudio2, Flags, XAudio2Processor);
    }
    
    if (SUCCEEDED(hr)) {
        *out = xaudio2;
    }
    else if (xaudio2) {
        xaudio2->lpVtbl->Release(xaudio2);
    }
    return hr;
}

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

static void nop(IXAudio2EngineCallback *this)
{
}

static bool _audio_init(void);

static _Atomic bool needs_restart = false;

static void critical_error(IXAudio2EngineCallback *this, HRESULT error)
{
    needs_restart = true;
}


static size_t _audio_get_queue_length(void)
{
    if (needs_restart) {
        _audio_init();
        if (!xaudio2) return 0;
        _audio_set_paused(!playing);
    }
    static XAUDIO2_VOICE_STATE state;
    IXAudio2SourceVoice_GetState(source_voice, &state);
    
    return state.BuffersQueued * BATCH_SIZE + (pos & (BATCH_SIZE - 1));
}

static void _audio_queue_sample(GB_sample_t *sample)
{
    if (!playing) return;
    
    if (needs_restart) {
        _audio_init();
        if (!xaudio2) return;
        _audio_set_paused(!playing);
    }
        
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
    if (needs_restart) {
        needs_restart = false;
        if (xaudio2) {
            xaudio2->lpVtbl->Release(xaudio2);
            xaudio2 = NULL;
        }
    }
    
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
                                       NULL // Effect chain
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
    
    
    static IXAudio2EngineCallbackVtbl callbacks = {
        nop,
        nop,
        .OnCriticalError = critical_error
    };
    
    static IXAudio2EngineCallback callbackObject = {
        .lpVtbl = &callbacks
    };
    
    IXAudio2SourceVoice_RegisterForCallbacks(xaudio2, &callbackObject);
    
    return true;
}

static void _audio_deinit(void)
{
    _audio_set_paused(true);
}

GB_AUDIO_DRIVER(XAudio2_7);
