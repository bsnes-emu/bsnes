import sys, time, array, ctypes, os

libsnes = ctypes.cdll.LoadLibrary("/usr/local/lib/libsnes.so")

snes_video_refresh_t = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint16), ctypes.c_uint, ctypes.c_uint)
snes_set_video_refresh = libsnes.snes_set_video_refresh
snes_set_video_refresh.restype = None
snes_set_video_refresh.argtypes = [snes_video_refresh_t]

snes_audio_sample_t = ctypes.CFUNCTYPE(None, ctypes.c_uint16, ctypes.c_uint16)
snes_set_audio_sample = libsnes.snes_set_audio_sample
snes_set_audio_sample.restype = None
snes_set_audio_sample.argtypes = [snes_audio_sample_t]

snes_input_poll_t = ctypes.CFUNCTYPE(None)
snes_set_input_poll = libsnes.snes_set_input_poll
snes_set_input_poll.restype = None
snes_set_input_poll.argtypes = [snes_input_poll_t]

snes_input_state_t = ctypes.CFUNCTYPE(ctypes.c_int16, ctypes.c_bool, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint)
snes_set_input_state = libsnes.snes_set_input_state
snes_set_input_state.restype = ctypes.c_int16
snes_set_input_state.argtypes = [snes_input_state_t]

snes_library_revision = libsnes.snes_library_revision
snes_library_revision.restype = ctypes.c_uint
snes_library_revision.argtypes = []

snes_init = libsnes.snes_init
snes_init.restype = None
snes_init.argtypes = []

snes_term = libsnes.snes_term
snes_term.restype = None
snes_term.argtypes = []

snes_power = libsnes.snes_power
snes_power.restype = None
snes_power.argtypes = []

snes_reset = libsnes.snes_reset
snes_reset.restype = None
snes_reset.argtypes = []

snes_run = libsnes.snes_run
snes_run.restype = None
snes_run.argtypes = []

snes_unload = libsnes.snes_unload
snes_unload.restype = None
snes_unload.argtypes = []

snes_serialize_size = libsnes.snes_serialize_size
snes_serialize_size.restype = ctypes.c_uint
snes_serialize_size.argtypes = []

snes_serialize = libsnes.snes_serialize
snes_serialize.restype = ctypes.c_bool
snes_serialize.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint]

snes_unserialize = libsnes.snes_unserialize
snes_unserialize.restype = ctypes.c_bool
snes_unserialize.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint]

snes_cheat_reset = libsnes.snes_cheat_reset
snes_cheat_reset.restype = None
snes_cheat_reset.argtypes = []

snes_cheat_set = libsnes.snes_cheat_set
snes_cheat_set.restype = None
snes_cheat_set.argtypes = [ctypes.c_uint, ctypes.c_bool, ctypes.POINTER(ctypes.c_char)]

snes_load_cartridge_normal = libsnes.snes_load_cartridge_normal
snes_load_cartridge_normal.restype = None
snes_load_cartridge_normal.argtypes = [ctypes.POINTER(ctypes.c_char), ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint]

snes_get_region = libsnes.snes_get_region
snes_get_region.restype = ctypes.c_bool
snes_get_region.argtypes = []

snes_get_memory_data = libsnes.snes_get_memory_data
snes_get_memory_data.restype = ctypes.POINTER(ctypes.c_uint8)
snes_get_memory_data.argtypes = [ctypes.c_uint]

snes_get_memory_size = libsnes.snes_get_memory_size
snes_get_memory_size.restype = ctypes.c_uint
snes_get_memory_size.argtypes = [ctypes.c_uint]

snes_blit_set_colortable = libsnes.snes_blit_set_colortable
snes_blit_set_colortable.restype = None
snes_blit_set_colortable.argtypes = [ctypes.POINTER(ctypes.c_uint32)]

snes_blit = libsnes.snes_blit
snes_blit.restype = None
snes_blit.argtypes = [ctypes.POINTER(ctypes.c_uint32), ctypes.c_uint, ctypes.c_uint, ctypes.c_uint, ctypes.POINTER(ctypes.c_uint16), ctypes.c_uint, ctypes.c_uint, ctypes.c_uint]

class SNES:
	def __init__(self):
		self.loaded = False

	def library_revision(self):
		return snes_library_revision()

	def set_video_refresh(self, callback):
		self.video_refresh = snes_video_refresh_t(callback)
		snes_set_video_refresh(self.video_refresh)

	def set_audio_sample(self, callback):
		self.audio_sample = snes_audio_sample_t(callback)
		snes_set_audio_sample(self.audio_sample)

	def set_input_poll(self, callback):
		self.input_poll = snes_input_poll_t(callback)
		snes_set_input_poll(self.input_poll)

	def set_input_state(self, callback):
		self.input_state = snes_input_state_t(callback)
		snes_set_input_state(self.input_state)

	def init(self):
		snes_init()

	def term(self):
		snes_term()

	def power(self):
		snes_power()

	def reset(self):
		snes_reset()

	def run(self):
		snes_run()

	def unload(self):
		snes_unload()
		self.loaded = False

	def serialize_size(self):
		return snes_serialize_size()

	def load_cartridge_normal(self, filename):
		size = os.path.getsize(filename)
		fp = open(filename, "rb")
		data = fp.read()
		snes_load_cartridge_normal(None, ctypes.cast(data, ctypes.POINTER(ctypes.c_uint8)), size)
		fp.close()
		self.loaded = True

	def get_region(self):
		return snes_get_region()

	def set_colortable(self, colortable):
		snes_blit_set_colortable(ctypes.cast(colortable.buffer_info()[0], ctypes.POINTER(ctypes.c_uint32)))

	def blit(self, outbuffer, outpitch, outwidth, outheight, inbuffer, inpitch, width, height):
		snes_blit(ctypes.cast(outbuffer.buffer_info()[0], ctypes.POINTER(ctypes.c_uint32)), outpitch, outwidth, outheight, inbuffer, inpitch, width, height)
