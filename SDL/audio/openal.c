#include "audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN_MS 32

static ALCdevice *al_device = NULL;
static ALCcontext *al_context = NULL;
static GB_sample_t *audio_buffer = NULL;
static ALuint al_source = 0;
static ALCint sample_rate = 0;
static unsigned buffer_size = 0;
static unsigned buffer_pos = 0;

#define AL_ERR_STRINGIFY(x) #x
#define AL_ERR_TOSTRING(x) AL_ERR_STRINGIFY(x)
#define AL_ERROR(msg) check_al_error(msg, __FILE__ ":" AL_ERR_TOSTRING(__LINE__))

// Check if the previous OpenAL call returned an error.
// If an error occurred a message will be logged to stderr.
bool check_al_error(const char *user_msg, const char *file) {
    ALCenum error = alGetError();
    char *description;

    switch (error) {
        case AL_NO_ERROR:
            return false;
        case AL_INVALID_NAME:
            description = "A bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            description = "An invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            description = "An invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            description = "The requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            description = "The requested operation resulted in OpenAL running out of memory";
            break;
    }

    if (user_msg != NULL) {
        fprintf(stderr, "[%s] %s: %s\n", file, user_msg, description);
    }
    else {
        fprintf(stderr, "[%s] %s\n", file, description);
    }

    return true;
}

static void _audio_deinit(void) {
    // Stop the source (this should mark all queued buffers as processed)
    alSourceStop(al_source);

    // Check if there are buffers that can be freed
    ALint processed;
    alGetSourcei(al_source, AL_BUFFERS_PROCESSED, &processed);
    if (!AL_ERROR("Failed to query number of processed buffers")) {
        // Try to free the buffers, we do not care about potential errors here
        while (processed--) {
            ALuint buffer;
            alSourceUnqueueBuffers(al_source, 1, &buffer);
            alDeleteBuffers(1, &buffer);
        }
    }

    alDeleteSources(1, &al_source);
    alcDestroyContext(al_context);
    alcCloseDevice(al_device);

    if (audio_buffer != NULL) {
        free(audio_buffer);
        audio_buffer = NULL;
    }
}

static void free_processed_buffers(void)
{
    ALint processed;
    alGetSourcei(al_source, AL_BUFFERS_PROCESSED, &processed);
    if (AL_ERROR("Failed to query number of processed buffers")) {
        _audio_deinit();
        return;
    }

    while (processed--) {
        ALuint buffer;

        alSourceUnqueueBuffers(al_source, 1, &buffer);
        if (AL_ERROR("Failed to unqueue buffer")) {
            _audio_deinit();
            return;
        }

        alDeleteBuffers(1, &buffer);
        if (AL_ERROR("Failed to delete buffer")) {
            _audio_deinit();
            return;
        }
    }
}

static bool _audio_is_playing(void)
{
    ALenum state;
    alGetSourcei(al_source, AL_SOURCE_STATE, &state);
    if (AL_ERROR("Failed to query source state")) {
        _audio_deinit();
        return false;
    }

    return state == AL_PLAYING;
}

static void _audio_clear_queue(void)
{
    alSourceStop(al_source);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return;
    }

    free_processed_buffers();
}

static void _audio_set_paused(bool paused)
{
    if (paused) {
        alSourcePause(al_source);
    }
    else {
        alSourcePlay(al_source);
    }

    if (AL_ERROR(NULL)) {
        _audio_deinit();
    }
}

static unsigned _audio_get_frequency(void)
{
    return sample_rate;
}

static size_t _audio_get_queue_length(void)
{
    // Get the number of all attached buffers
    ALint buffers;
    alGetSourcei(al_source, AL_BUFFERS_QUEUED, &buffers);
    if (AL_ERROR("Failed to query number of queued buffers")) {
        buffers = 0;
    }

    // Get the number of all processed buffers (ready to be detached)
    ALint processed;
    alGetSourcei(al_source, AL_BUFFERS_PROCESSED, &processed);
    if (AL_ERROR("Failed to query number of processed buffers")) {
        processed = 0;
    }

    return (buffers - processed) * buffer_size * sizeof(GB_sample_t);
}

static void _audio_queue_sample(GB_sample_t *sample)
{
    audio_buffer[buffer_pos++] = *sample;

    if (buffer_pos == buffer_size) {
        buffer_pos = 0;

        ALuint al_buffer;
        alGenBuffers(1, &al_buffer);
        if (AL_ERROR("Failed to create audio buffer")) {
            return _audio_deinit();
        }

        alBufferData(al_buffer, AL_FORMAT_STEREO16, audio_buffer, buffer_size * sizeof(GB_sample_t), sample_rate);
        if (AL_ERROR("Failed to buffer data")) {
            return _audio_deinit();
        }

        alSourceQueueBuffers(al_source, 1, &al_buffer);
        if (AL_ERROR("Failed to queue buffer")) {
            return _audio_deinit();
        }

        // In case of an audio underrun, the source might
        // have finished playing all attached buffers
        // which means its status will be "AL_STOPPED".
        if (!_audio_is_playing()) {
            alSourcePlay(al_source);
        }

        free_processed_buffers();
    }
}

static bool _audio_init(void)
{
    // Open the default device
    al_device = alcOpenDevice(NULL);
    if (!al_device) {
        AL_ERROR("Failed to open device");
        return false;
    }

    // Create a new audio context without special attributes
    al_context = alcCreateContext(al_device, NULL);
    if (al_context == NULL) {
        AL_ERROR("Failed to create context");
        _audio_deinit();
        return false;
    }

    // Enable our audio context
    if (!alcMakeContextCurrent(al_context)) {
        AL_ERROR("Failed to set context");
        _audio_deinit();
        return false;
    }

    // Query the sample rate of the playback device
    alcGetIntegerv(al_device, ALC_FREQUENCY, 1, &sample_rate);
    if (AL_ERROR("Failed to query sample rate")) {
        _audio_deinit();
        return false;
    }

    // Allocate our working buffer
    buffer_size = (sample_rate * BUFFER_LEN_MS) / 1000;
    audio_buffer = malloc(buffer_size * sizeof(GB_sample_t));
    if (audio_buffer == NULL) {
        fprintf(stderr, "Failed to allocate audio buffer\n");
        _audio_deinit();
        return false;
    }

    // Create our playback source
    alGenSources(1, &al_source);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return false;
    }

    // Keep the pitch as is
    alSourcef(al_source, AL_PITCH, 1);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return false;
    }

    // Keep the volume as is
    alSourcef(al_source, AL_GAIN, 1);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return false;
    }

    // Position our source at the center of the 3D space
    alSource3f(al_source, AL_POSITION, 0, 0, 0);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return false;
    }

    // Our source is fixed in space
    alSource3f(al_source, AL_VELOCITY, 0, 0, 0);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return false;
    }

    // Our source does not loop
    alSourcei(al_source, AL_LOOPING, AL_FALSE);
    if (AL_ERROR(NULL)) {
        _audio_deinit();
        return false;
    }

    return true;
}

GB_AUDIO_DRIVER(OpenAL);
