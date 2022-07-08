#define INITGUID
#include <Windows.h>

/* Minimal definitions for XAudio2.7 */
typedef UINT32 XAUDIO2_PROCESSOR;

typedef struct XAUDIO2_BUFFER {
    UINT32 Flags;
    UINT32 AudioBytes;
    const BYTE *pAudioData;
    UINT32 PlayBegin;
    UINT32 PlayLength;
    UINT32 LoopBegin;
    UINT32 LoopLength;
    UINT32 LoopCount;
    void *pContext;
} XAUDIO2_BUFFER;

typedef struct XAUDIO2_VOICE_STATE {
    void *pCurrentBufferContext;
    UINT32 BuffersQueued;
    UINT64 SamplesPlayed;
} XAUDIO2_VOICE_STATE;

typedef struct IXAudio2SourceVoice {
    struct IXAudio2SourceVoiceVtbl *lpVtbl;
} IXAudio2SourceVoice;

typedef struct IXAudio2SourceVoiceVtbl IXAudio2SourceVoiceVtbl;

#undef INTERFACE
#define INTERFACE IXAudio2SourceVoice

struct IXAudio2SourceVoiceVtbl {
    void *voiceMethods[19]; // Unused inherited methods
    STDMETHOD(Start) (THIS_ UINT32 Flags, UINT32 OperationSet) PURE;
    STDMETHOD(Stop) (THIS_ UINT32 Flags, UINT32 OperationSet) PURE;
    STDMETHOD(SubmitSourceBuffer) (THIS_ __in const XAUDIO2_BUFFER *pBuffer, __in_opt const void *pBufferWMA) PURE;
    STDMETHOD(FlushSourceBuffers) (THIS) PURE;
    STDMETHOD(Discontinuity) (THIS) PURE;
    STDMETHOD(ExitLoop) (THIS_ UINT32 OperationSet) PURE;
    STDMETHOD_(void, GetState) (THIS_ __out XAUDIO2_VOICE_STATE *pVoiceState) PURE;
};

typedef struct IXAudio2 {
    struct IXAudio2Vtbl *lpVtbl;
} IXAudio2;

typedef struct IXAudio2Vtbl IXAudio2Vtbl;
typedef void *IXAudio2MasteringVoice;

#undef INTERFACE
#define INTERFACE IXAudio2

struct IXAudio2Vtbl {
    void *QueryInterface;
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;
    void *GetDeviceCount;
    void *GetDeviceDetails;
    STDMETHOD(Initialize) (THIS_ UINT32 Flags,
                           XAUDIO2_PROCESSOR XAudio2Processor) PURE;

     void *RegisterForCallbacks;
     void *UnregisterForCallbacks;
    
    STDMETHOD(CreateSourceVoice) (THIS_ __deref_out IXAudio2SourceVoice **ppSourceVoice,
                                  __in const WAVEFORMATEX *pSourceFormat,
                                  UINT32 Flags,
                                  float MaxFrequencyRatio,
                                  __in_opt void *pCallback,
                                  __in_opt const void *pSendList,
                                  __in_opt const void *pEffectChain) PURE;

    void *CreateSubmixVoice;
    
    STDMETHOD(CreateMasteringVoice) (THIS_ __deref_out IXAudio2MasteringVoice **ppMasteringVoice,
                                     UINT32 InputChannels,
                                     UINT32 InputSampleRate,
                                     UINT32 Flags, UINT32 DeviceIndex,
                                     __in_opt const void *pEffectChain) PURE;
};

#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
DEFINE_GUID(CLSID_##className, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)

#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
DEFINE_GUID(IID_##interfaceName, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)

DEFINE_CLSID(XAudio2, 5a508685, a254, 4fba, 9b, 82, 9a, 24, b0, 03, 06, af);
DEFINE_IID(IXAudio2, 8bcf1f58, 9fe7, 4583, 8a, c6, e2, ad, c4, 65, c8, bb);


#define IXAudio2SourceVoice_Start(This,Flags,OperationSet) ((This)->lpVtbl->Start(This,Flags,OperationSet))
#define IXAudio2SourceVoice_Stop(This,Flags,OperationSet) ((This)->lpVtbl->Stop(This,Flags,OperationSet))
#define IXAudio2SourceVoice_SubmitSourceBuffer(This,pBuffer,pBufferWMA) ((This)->lpVtbl->SubmitSourceBuffer(This,pBuffer,pBufferWMA))
#define IXAudio2SourceVoice_FlushSourceBuffers(This) ((This)->lpVtbl->FlushSourceBuffers(This))
#define IXAudio2SourceVoice_GetState(This,pVoiceState) ((This)->lpVtbl->GetState(This,pVoiceState))
#define IXAudio2_CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceIndex,pEffectChain) ((This)->lpVtbl->CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceIndex,pEffectChain))
#define IXAudio2_CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain) ((This)->lpVtbl->CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain))

#define XAUDIO2_COMMIT_NOW 0
#define XAUDIO2_DEFAULT_PROCESSOR 0xffffffff
#define XAUDIO2_DEFAULT_FREQ_RATIO      2.0f 

// WASAPI extras. This is a hack, but Windows itself is a hack so I don't care
DEFINE_CLSID(MMDeviceEnumerator, bcde0395, e52f, 467c, 8e, 3d, c4, 57, 92, 91, 69, 2e);
DEFINE_IID(IMMDeviceEnumerator,  a95664d2, 9614, 4f35, a7, 46, de, 8d, b6, 36, 17, e6);
