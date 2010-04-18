#!/usr/bin/python
import sys, time, array, ctypes, os, ao
from PyQt4 import QtCore
from PyQt4 import QtGui
from PyQt4 import QtOpenGL
from OpenGL.GL import *
from libsnes import SNES

snes = SNES()

class Canvas(QtOpenGL.QGLWidget):
	def __init__(self, parent = None):
		self.videobuffer = array.array('I', [0] * 512 * 512)
		self.colortable = array.array('I', [0] * 32 * 32 * 32)
		offset = 0
		for r in xrange(32):
			for g in xrange(32):
				for b in xrange(32):
					self.colortable[offset] = (b << 19) + (g << 11) + (r << 3)
					offset += 1
		snes.set_colortable(self.colortable)

		self.rasterWidth = 256
		self.rasterHeight = 224

		QtOpenGL.QGLWidget.__init__(self, parent)
		self.setMinimumSize(512, 448)

	def videoRefresh(self, data, width, height):
		pitch = 2048
		if height >= 448: pitch = 1024
		if height == 239: height = 224
		if height == 478: height = 448

		self.rasterWidth = width
		self.rasterHeight = height

		snes.blit(self.videobuffer, 2048, width, height, data, pitch, width, height)
		self.update()

	def paintGL(self):
		outputWidth = self.width()
		outputHeight = self.height()

		textureWidth = 512
		textureHeight = 512

		glMatrixMode(GL_PROJECTION)
		glLoadIdentity()
		glOrtho(0, outputWidth, 0, outputHeight, -1.0, 1.0)
		glViewport(0, 0, outputWidth, outputHeight)

		glMatrixMode(GL_MODELVIEW)
		glLoadIdentity()

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, self.rasterWidth, self.rasterHeight, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, self.videobuffer.tostring())

		w = 1.0 * self.rasterWidth / textureWidth
		h = 1.0 * self.rasterHeight / textureHeight
		u = outputWidth
		v = outputHeight

		glBegin(GL_TRIANGLE_STRIP)
		glTexCoord2f(0, 0); glVertex3i(0, v, 0)
		glTexCoord2f(w, 0); glVertex3i(u, v, 0)
		glTexCoord2f(0, h); glVertex3i(0, 0, 0)
		glTexCoord2f(w, h); glVertex3i(u, 0, 0)
		glEnd()

	def initializeGL(self):
		glDisable(GL_ALPHA_TEST)
		glDisable(GL_BLEND)
		glDisable(GL_DEPTH_TEST)
		glDisable(GL_POLYGON_SMOOTH)
		glDisable(GL_STENCIL_TEST)
		glEnable(GL_DITHER)
		glEnable(GL_TEXTURE_2D)
		glClearColor(0.0, 0.0, 0.0, 0.0)

		glBindTexture(GL_TEXTURE_2D, glGenTextures(1))
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 512)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, self.videobuffer.tostring())

class MainWindow(QtGui.QMainWindow):
	def __init__(self, parent = None):
		QtGui.QMainWindow.__init__(self, parent)
		self.setWindowTitle("bsnes/PyQt")

		self.menu_system = self.menuBar().addMenu("&System")

		self.menu_system_loadCartridge = QtGui.QAction("Load Cartridge ...", self)
		self.connect(self.menu_system_loadCartridge, QtCore.SIGNAL('triggered()'), self.loadCartridgeEvent)
		self.menu_system.addAction(self.menu_system_loadCartridge)

		self.menu_system_quit = QtGui.QAction("Quit", self)
		self.connect(self.menu_system_quit, QtCore.SIGNAL('triggered()'), QtCore.SLOT('close()'))
		self.menu_system.addAction(self.menu_system_quit)

		self.menu_settings = self.menuBar().addMenu("S&ettings")
		self.menu_tools = self.menuBar().addMenu("&Tools")
		self.menu_help = self.menuBar().addMenu("&Help")

		self.canvas = Canvas()
		self.setCentralWidget(self.canvas)

		self.resize(0, 0)
		self.show()

		self.audio = ao.AudioDevice("alsa", bits = 16, rate = 32000, channels = 2)

		snes.init()
		snes.set_video_refresh(self.canvas.videoRefresh)
		snes.set_audio_sample(self.audioSample)

		if len(sys.argv) == 2:
			snes.load_cartridge_normal(sys.argv[1])

	def audioSample(self, left, right):
		self.audio.play(array.array('H', [left, right]), 4)

	def loadCartridgeEvent(self):
		filename = QtGui.QFileDialog.getOpenFileName(self, "Load SNES Cartridge", "", "SNES cartridges (*.sfc)")
		if len(filename) > 0:
			snes.load_cartridge_normal(filename)

app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
while mainWindow.isVisible() == True:
	app.processEvents()
	if snes.loaded == True:
		snes.run()
