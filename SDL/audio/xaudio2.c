#include "audio.h"
#include <Windows.h>
#include <xaudio2.h>

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

bool GB_audio_is_playing(void)
{
    return playing;
}

void GB_audio_set_paused(bool paused)
{
    if (paused) {
        playing = false;
        IXAudio2SourceVoice_Stop(source_voice, 0, XAUDIO2_COMMIT_NOW);
        GB_audio_clear_queue();
    }
    else {
        playing = true;
        IXAudio2SourceVoice_Start(source_voice, 0, XAUDIO2_COMMIT_NOW);
    }
    
}

void GB_audio_clear_queue(void)
{
    pos = 0;
    IXAudio2SourceVoice_FlushSourceBuffers(source_voice);
}

unsigned GB_audio_get_frequency(void)
{
    return AUDIO_FREQUENCY;
}

size_t GB_audio_get_queue_length(void)
{
    static XAUDIO2_VOICE_STATE state;
    IXAudio2SourceVoice_GetState(source_voice, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    
    return state.BuffersQueued * BATCH_SIZE + (pos & (BATCH_SIZE - 1));
}

void GB_audio_queue_sample(GB_sample_t *sample)
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

void GB_audio_init(void)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        fprintf(stderr, "CoInitializeEx failed: %lx\n", hr);
        return;
    }
    
    hr = XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        fprintf(stderr, "XAudio2Create failed: %lx\n", hr);
        return;
    }
    
    hr = IXAudio2_CreateMasteringVoice(xaudio2, &master_voice,
                                       2, // 2 channels
                                       AUDIO_FREQUENCY,
                                       0, // Flags
                                       0, // Device index
                                       NULL, // Effect chain
                                       AudioCategory_GameMedia // Category
                                      );
    if (FAILED(hr)) {
        fprintf(stderr, "CreateMasteringVoice failed: %lx\n", hr);
        return;
    }
    
    hr = IXAudio2_CreateSourceVoice(xaudio2, &source_voice, &wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    
    if (FAILED(hr)) {
        fprintf(stderr, "CreateSourceVoice failed: %lx\n", hr);
        return;
    }
}
