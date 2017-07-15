using ASIOError = long;
enum : long {
  ASE_OK = 0,
  ASE_SUCCESS = 0x3f4847a0,
  ASE_NotPresent = -1000,
  ASE_HWMalfunction,
  ASE_InvalidParameter,
  ASE_InvalidMode,
  ASE_SPNotAdvancing,
  ASE_NoClock,
  ASE_NoMemory,
};

using ASIOBool = long;
enum : long {
  ASIOFalse = 0,
  ASIOTrue = 1,
};

using ASIOSampleRate = double;
using ASIOSamples = long long int;
using ASIOTimeStamp = long long int;

using ASIOSampleType = long;
enum : long {
  ASIOSTInt16MSB = 0,
  ASIOSTInt24MSB = 1,
  ASIOSTInt32MSB = 2,
  ASIOSTFloat32MSB = 3,
  ASIOSTFloat64MSB = 4,

  ASIOSTInt32MSB16 = 8,
  ASIOSTInt32MSB18 = 9,
  ASIOSTInt32MSB20 = 10,
  ASIOSTInt32MSB24 = 11,

  ASIOSTInt16LSB = 16,
  ASIOSTInt24LSB = 17,
  ASIOSTInt32LSB = 18,
  ASIOSTFloat32LSB = 19,
  ASIOSTFloat64LSB = 20,

  ASIOSTInt32LSB16 = 24,
  ASIOSTInt32LSB18 = 25,
  ASIOSTInt32LSB20 = 26,
  ASIOSTInt32LSB24 = 27,

  ASIOSTDSDInt8LSB1 = 32,
  ASIOSTDSDInt8MSB1 = 33,
  ASIOSTDSDInt8NER8 = 40,

  ASIOSTLastEntry,
};

struct ASIODriverInfo {
  long asioVersion;
  long driverVersion;
  char name[32];
  char errorMessage[124];
  void* sysRef;
};

struct ASIOBufferInfo {
  ASIOBool isInput;
  long channelNum;
  void* buffers[2];
};

struct ASIOChannelInfo {
  long channel;
  ASIOBool isInput;
  ASIOBool isActive;
  long channelGroup;
  ASIOSampleType type;
  char name[32];
};

struct ASIOClockSource {
  long index;
  long associatedChannel;
  long associatedGroup;
  ASIOBool isCurrentSource;
  char name[32];
};

struct ASIOTimeInfo {
  double speed;
  ASIOTimeStamp systemTime;
  ASIOSamples samplePosition;
  ASIOSampleRate sampleRate;
  unsigned long flags;
  char reserved[12];
};
enum : unsigned long {
  kSystemTimeValid = 1 << 0,
  kSamplePositionValid = 1 << 1,
  kSampleRateValid = 1 << 2,
  kSpeedValid = 1 << 3,
  kSampleRateChanged = 1 << 4,
  kClockSourceChanged = 1 << 5,
};

struct ASIOTimeCode {
  double speed;
  ASIOSamples timeCodeSamples;
  unsigned long flags;
  char future[64];
};
enum : unsigned long {
  kTcValid = 1 << 0,
  kTcRunning = 1 << 1,
  kTcReverse = 1 << 2,
  kTcOnspeed = 1 << 3,
  kTcStill = 1 << 4,
  kTcSpeedValid = 1 << 8,
};

struct ASIOTime {
  long reserved[4];
  ASIOTimeInfo timeInfo;
  ASIOTimeCode timeCode;
};

struct ASIOCallbacks {
  auto (*bufferSwitch)(long doubleBufferIndex, ASIOBool directProcess) -> void;
  auto (*sampleRateDidChange)(ASIOSampleRate sRate) -> void;
  auto (*asioMessage)(long selector, long value, void* message, double* opt) -> long;
  auto (*bufferSwitchTimeInfo)(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess) -> ASIOTime*;
};
enum : long {
  kAsioSelectorSupported = 1,
  kAsioEngineVersion,
  kAsioResetRequest,
  kAsioBufferSizeChange,
  kAsioResyncRequest,
  kAsioLatenciesChanged,
  kAsioSupportsTimeInfo,
  kAsioSupportsTimeCode,
  kAsioMMCCommand,
  kAsioSupportsInputMonitor,
  kAsioSupportsInputGain,
  kAsioSupportsInputMeter,
  kAsioSupportsOutputGain,
  kAsioSupportsOutputMeter,
  kAsioOverload,
  kAsioNumMessageSelectors,
};

struct IASIO : public IUnknown {
  virtual auto init(void* sysHandle) -> ASIOBool;
  virtual auto getDriverName(char* name) -> void;
  virtual auto getDriverVersion() -> long;
  virtual auto getErrorMessage(char* error) -> void;
  virtual auto start() -> ASIOError;
  virtual auto stop() -> ASIOError;
  virtual auto getChannels(long* numInputChannels, long* numOutputChannels) -> ASIOError = 0;
  virtual auto getLatencies(long* inputLatency, long* outputLatency) -> ASIOError = 0;
  virtual auto getBufferSize(long* minSize, long* maxSize, long* preferredSize, long* granularity) -> ASIOError = 0;
  virtual auto canSampleRate(ASIOSampleRate sampleRate) -> ASIOError = 0;
  virtual auto getSampleRate(ASIOSampleRate* sampleRate) -> ASIOError = 0;
  virtual auto setSampleRate(ASIOSampleRate sampleRate) -> ASIOError = 0;
  virtual auto getClockSources(ASIOClockSource* clocks, long* numSources) -> ASIOError = 0;
  virtual auto setClockSource(long reference) -> ASIOError = 0;
  virtual auto getSamplePosition(ASIOSamples* sPos, ASIOTimeStamp* tStamp) -> ASIOError = 0;
  virtual auto getChannelInfo(ASIOChannelInfo* info) -> ASIOError = 0;
  virtual auto createBuffers(ASIOBufferInfo* bufferInfos, long numChannels, long bufferSize, ASIOCallbacks* callbacks) -> ASIOError = 0;
  virtual auto disposeBuffers() -> ASIOError = 0;
  virtual auto controlPanel() -> ASIOError = 0;
  virtual auto future(long selector, void* opt) -> ASIOError = 0;
  virtual auto outputReady() -> ASIOError = 0;
};
