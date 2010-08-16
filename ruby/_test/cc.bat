@g++ -std=gnu++0x -O3 -fomit-frame-pointer -I../.. -o test test.cpp -DAUDIO_DIRECTSOUND -DAUDIO_XAUDIO2 -DINPUT_DIRECTINPUT -lole32 -luuid -ldxguid -ldsound -ldinput8
@pause
