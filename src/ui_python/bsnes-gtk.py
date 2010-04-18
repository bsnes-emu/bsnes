#!/usr/bin/python
import sys, time, array, ctypes, os, ao
import pygtk
pygtk.require("2.0")
import gtk, gobject
from libsnes import SNES

snes = SNES()

class MainWindow(gtk.Window):
	def __init__(self):
		gtk.Window.__init__(self, gtk.WINDOW_TOPLEVEL)

		self.keystate_up = 0
		self.keystate_down = 0
		self.keystate_left = 0
		self.keystate_right = 0
		self.keystate_a = 0
		self.keystate_b = 0
		self.keystate_x = 0
		self.keystate_y = 0
		self.keystate_l = 0
		self.keystate_r = 0
		self.keystate_select = 0
		self.keystate_start = 0

		self.set_title("bsnes/PyGTK")
		self.set_position(gtk.WIN_POS_CENTER)
		self.set_resizable(False)
		self.connect("key-press-event", self.key_press)
		self.connect("key-release-event", self.key_release)
		self.connect("destroy", self.destroy)

		self.box = gtk.VBox()
		self.add(self.box)

		self.menu = gtk.MenuBar()

		self.system_menu = gtk.Menu()
		self.system_menuitem = gtk.MenuItem("System")
		self.system_menuitem.set_submenu(self.system_menu)

		self.system_load = gtk.MenuItem("Load Cartridge")
		self.system_load.connect("activate", self.load_cartridge)
		self.system_menu.append(self.system_load)

		self.system_quit = gtk.MenuItem("Quit")
		self.system_quit.connect("activate", gtk.main_quit)
		self.system_menu.append(self.system_quit)

		self.settings_menu = gtk.Menu()
		self.settings_menuitem = gtk.MenuItem("Settings")
		self.settings_menuitem.set_submenu(self.settings_menu)

		self.tools_menu = gtk.Menu()
		self.tools_menuitem = gtk.MenuItem("Tools")
		self.tools_menuitem.set_submenu(self.tools_menu)

		self.help_menu = gtk.Menu()
		self.help_menuitem = gtk.MenuItem("Help")
		self.help_menuitem.set_submenu(self.help_menu)

		self.menu.append(self.system_menuitem)
		self.menu.append(self.settings_menuitem)
		self.menu.append(self.tools_menuitem)
		self.menu.append(self.help_menuitem)
		self.box.pack_start(self.menu, False, False, 0)

		self.canvas = gtk.DrawingArea()
		self.canvas.modify_bg(gtk.STATE_NORMAL, gtk.gdk.Color(0, 0, 0))
		self.canvas.set_size_request(512, 448)
		self.box.pack_start(self.canvas)

		self.videobuffer = array.array('B', [0] * 512 * 448 * 4)
		self.audiobuffer = array.array('H', [0] * 256 * 2)
		self.audiolength = 0
		self.audio = ao.AudioDevice("alsa", bits = 16, rate = 32000, channels = 2)

		self.show_all()

		snes.init()
		snes.set_video_refresh(self.video_refresh)
		snes.set_audio_sample(self.audio_sample)
		snes.set_input_poll(self.input_poll)
		snes.set_input_state(self.input_state)

		colortable = array.array('I', [0] * 32 * 32 * 32)
		offset = 0
		for r in xrange(32):
			for g in xrange(32):
				for b in xrange(32):
					colortable[offset] = (r << 19) + (g << 11) + (b << 3)
					offset += 1
		snes.set_colortable(colortable)

		if len(sys.argv) == 2:
			snes.load_cartridge_normal(sys.argv[1])

	def key_press(self, widget, event):
		if event.keyval == 65362: self.keystate_up = 1
		if event.keyval == 65364: self.keystate_down = 1
		if event.keyval == 65361: self.keystate_left = 1
		if event.keyval == 65363: self.keystate_right = 1
		if event.keyval ==   122: self.keystate_b = 1
		if event.keyval ==   120: self.keystate_a = 1
		if event.keyval ==    97: self.keystate_y = 1
		if event.keyval ==   115: self.keystate_x = 1
		if event.keyval ==   100: self.keystate_l = 1
		if event.keyval ==    99: self.keystate_r = 1
		if event.keyval ==    39: self.keystate_select = 1
		if event.keyval == 65293: self.keystate_start = 1

	def key_release(self, widget, event):
		if event.keyval == 65362: self.keystate_up = 0
		if event.keyval == 65364: self.keystate_down = 0
		if event.keyval == 65361: self.keystate_left = 0
		if event.keyval == 65363: self.keystate_right = 0
		if event.keyval ==   122: self.keystate_b = 0
		if event.keyval ==   120: self.keystate_a = 0
		if event.keyval ==    97: self.keystate_y = 0
		if event.keyval ==   115: self.keystate_x = 0
		if event.keyval ==   100: self.keystate_l = 0
		if event.keyval ==    99: self.keystate_r = 0
		if event.keyval ==    39: self.keystate_select = 0
		if event.keyval == 65293: self.keystate_start = 0

	def destroy(self, widget, data=None):
		snes.term()
		gtk.main_quit()

	def load_cartridge(self, widget):
		dialog = gtk.FileChooserDialog("Load Cartridge", None, gtk.FILE_CHOOSER_ACTION_OPEN,
			(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL, gtk.STOCK_OPEN, gtk.RESPONSE_OK))
		dialog.set_default_response(gtk.RESPONSE_OK)

		filter = gtk.FileFilter()
		filter.set_name("SNES cartridges")
		filter.add_pattern("*.sfc")
		filter.add_pattern("*.smc")
		filter.add_pattern("*.swc")
		filter.add_pattern("*.fig")
		dialog.add_filter(filter)

		response = dialog.run()
		if response == gtk.RESPONSE_OK:
			snes.unload()
			snes.load_cartridge_normal(dialog.get_filename())

		dialog.destroy()

	def run(self):
		if snes.loaded == True:
			snes.run()

	def video_refresh(self, data, width, height):
		# ignore overscan region to guarantee 1:1 pixel scaling
		if height == 239: height = 224
		if height == 478: height = 448
		pitch = 2048
		if height >= 448: pitch = 1024

		drawable = self.canvas.window
		gc = self.canvas.get_style().fg_gc[gtk.STATE_NORMAL]
		snes.blit(self.videobuffer, 2048, 512, 448, data, pitch, width, height)
		drawable.draw_rgb_32_image(gc, 0, 0, 512, 448, gtk.gdk.RGB_DITHER_NONE, self.videobuffer, -1)

	def audio_sample(self, left, right):
		self.audiobuffer[self.audiolength] = left;  self.audiolength += 1
		self.audiobuffer[self.audiolength] = right; self.audiolength += 1
		if self.audiolength == 256 * 2:
			self.audio.play(self.audiobuffer, 256 * 2 * 2)
			self.audiolength = 0

	def input_poll(self):
		pass

	def input_state(self, port, device, index, identifier):
		if port == 1: return 0

		if self.keystate_up == 1 and self.keystate_down == 1: self.keystate_down = 0
		if self.keystate_left == 1 and self.keystate_right == 1: self.keystate_right = 0

		if identifier ==  0: return self.keystate_b
		if identifier ==  1: return self.keystate_y
		if identifier ==  2: return self.keystate_select
		if identifier ==  3: return self.keystate_start
		if identifier ==  4: return self.keystate_up
		if identifier ==  5: return self.keystate_down
		if identifier ==  6: return self.keystate_left
		if identifier ==  7: return self.keystate_right
		if identifier ==  8: return self.keystate_a
		if identifier ==  9: return self.keystate_x
		if identifier == 10: return self.keystate_l
		if identifier == 11: return self.keystate_r

		return 0

mainWindow = MainWindow()
timer = gobject.timeout_add(10, lambda: mainWindow.run() or True)
gtk.main()
