#include "xaudio2_7.h"
#include "audio.h"


#define AUDIO_FREQUENCY 96000
static IXAudio2 *xaudio2 = NULL;
static IXAudio2MasteringVoice *master_voice = NULL;
static IXAudio2SourceVoice *source_voice = NULL;
static bool playing = false;
static GB_sample_t sample_pool[0x2000];
static unsigned pos = 0;

#define BATCH_SIZE 256


static const WAVEFORMATEX wave_format = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 2,
    .nSamplesPerSec = AUDIO_FREQUENCY,
    .nAvgBytesPerSec = AUDIO_FREQUENCY * 4,
    .nBlockAlign = 4,
    .wBitsPerSample = 16,
    .cbSize = 0
};

static inline HRESULT XAudio2Create(IXAudio2 **ppXAudio2,
                                        UINT32 Flags,
                                        XAUDIO2_PROCESSOR XAudio2Processor)
{
    IXAudio2 *pXAudio2;
    LoadLibraryEx("xaudio2_7.dll", NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    
    HRESULT hr = CoCreateInstance(&CLSID_XAudio2, NULL, CLSCTX_INPROC_SERVER, &IID_IXAudio2, (void**)&pXAudio2);
    if (SUCCEEDED(hr)) {
        hr = pXAudio2->lpVtbl->Initialize(pXAudio2, Flags, XAudio2Processor);
    }
    
    if (SUCCEEDED(hr)) {
        *ppXAudio2 = pXAudio2;
    }
    else {
        pXAudio2->lpVtbl->Release(pXAudio2);
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

static unsigned _audio_get_frequency(void)
{
    return AUDIO_FREQUENCY;
}

static size_t _audio_get_queue_length(void)
{
    static XAUDIO2_VOICE_STATE state;
    IXAudio2SourceVoice_GetState(source_voice, &state);
    
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
    
    hr = IXAudio2_CreateMasteringVoice(xaudio2, &master_voice,
                                       2, // 2 channels
                                       AUDIO_FREQUENCY,
                                       0, // Flags
                                       0, // Device index
                                       NULL // Effect chain
                                      );
    if (FAILED(hr)) {
        fprintf(stderr, "CreateMasteringVoice failed: %lx\n", hr);
        return false;
    }
    
    hr = IXAudio2_CreateSourceVoice(xaudio2, &source_voice, &wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    
    if (FAILED(hr)) {
        fprintf(stderr, "CreateSourceVoice failed: %lx\n", hr);
        return false;
    }
    
    return true;
}

GB_AUDIO_DRIVER(XAudio2_7);