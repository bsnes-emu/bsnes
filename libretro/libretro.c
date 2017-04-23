#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <stdarg.h>


#define AUDIO_FREQUENCY 44100

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define snprintf _snprintf
#endif

#include "gb.h"
#include "libretro.h"

#ifdef _WIN32
   char slash = '\\';
#else
   char slash = '/';
#endif

#define VIDEO_WIDTH 160
#define VIDEO_HEIGHT 144
#define VIDEO_PIXELS VIDEO_WIDTH * VIDEO_HEIGHT 

char battery_save_path[512]; 
char symbols_path[512];

static uint32_t *frame_buf;
static struct retro_log_callback logging;
static retro_log_printf_t log_cb;

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

signed short soundbuf[1024*2];

char retro_base_directory[4096];
char retro_game_path[4096];
int RLOOP=1;

GB_gameboy_t gb;

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}

static void replace_extension(const char *src, size_t length, char *dest, const char *ext)
{
    memcpy(dest, src, length);
    dest[length] = 0;

    /* Remove extension */
    for (size_t i = length; i--;) {
        if (dest[i] == '/') break;
        if (dest[i] == '.') {
            dest[i] = 0;
            break;
        }
    }

    /* Add new extension */
    strcat(dest, ext);
}

static void GB_update_keys_status(GB_gameboy_t *gb)
{
	int i;


	input_poll_cb();

	GB_set_key_state(gb, GB_KEY_RIGHT,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT));
	GB_set_key_state(gb, GB_KEY_LEFT, input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT));
	GB_set_key_state(gb, GB_KEY_UP,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) );
	GB_set_key_state(gb, GB_KEY_DOWN,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN));
	GB_set_key_state(gb, GB_KEY_A,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) );
	GB_set_key_state(gb, GB_KEY_B,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) );
	GB_set_key_state(gb, GB_KEY_SELECT,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT));
	GB_set_key_state(gb, GB_KEY_START,input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START) );

	GB_set_turbo_mode(gb, input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X),false );

	static bool ctrl = false;
	static bool shift = false;

	ctrl = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,RETROK_RCTRL) ||input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,RETROK_RCTRL);

	shift = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,RETROK_RSHIFT) ||input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,RETROK_LSHIFT);

	if(input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,RETROK_c))
		if(ctrl){
			ctrl=false;
			GB_debugger_break(gb);
		}


	for(i=0;i<10;i++)
		if(input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,RETROK_0+i)){
			if(ctrl){

				char save_path[strlen(retro_game_path) + 4];
                                char save_extension[] =".s0";
                                save_extension[2] += i;
                                replace_extension(retro_game_path, strlen(retro_game_path), save_path, save_extension);

                                if (shift) {
                                    GB_load_state(gb, save_path);
                                }
                                else {
                                    GB_save_state(gb, save_path);
                                }

				
			}
		}


}


static void audio_callback(void *gb)
{
    GB_apu_copy_buffer(gb, (GB_sample_t *) soundbuf, 735);
    audio_batch_cb(soundbuf, 735);
}


static void vblank(GB_gameboy_t *gb)
{
    GB_update_keys_status(gb);
    GB_set_pixels_output(gb, frame_buf);
    audio_callback(gb);
    RLOOP=0;
}

static uint32_t rgb_encode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return r<<16|g<<8|b;
}

static void debugger_interrupt(int ignore)
{
    /* ^C twice to exit */
    if (GB_debugger_is_stopped(&gb)) {
        exit(0);
    }
    GB_debugger_break(&gb);
}

static retro_environment_t environ_cb;

void retro_init(void)
{
   frame_buf = (uint32_t*)malloc(VIDEO_PIXELS * sizeof(uint32_t));
   const char *dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
   {
      snprintf(retro_base_directory, sizeof(retro_base_directory), "%s/sameboy", dir);
   }
   else snprintf(retro_base_directory, sizeof(retro_base_directory), "%s", ".");
}

void retro_deinit(void)
{
   free(frame_buf);
   frame_buf = NULL;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "sameboy";
   info->library_version  = "0.1";
   info->need_fullpath    = true;
   info->valid_extensions = "gb|gbc";
}


void retro_get_system_av_info(struct retro_system_av_info *info)
{
   struct retro_game_geometry geom = { VIDEO_WIDTH, VIDEO_HEIGHT,VIDEO_WIDTH, VIDEO_HEIGHT ,160.0 / 144.0 };
   struct retro_system_timing timing = { 60.0, 44100.0 };

   info->geometry = geom;
   info->timing   = timing;

}

static struct retro_rumble_interface rumble;

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;

   static const struct retro_controller_description controllers[] = {
      { "Nintendo DS", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0) },
   };

   static const struct retro_controller_info ports[] = {
      { controllers, 1 },
      { NULL, 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_reset(void)
{

}

static void check_variables(void)
{

}

void retro_run(void)
{
   bool updated = false;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();

   while(RLOOP==1)GB_run(&gb);
   RLOOP=1;

   video_cb(frame_buf, VIDEO_WIDTH, VIDEO_HEIGHT, 0);

}

bool retro_load_game(const struct retro_game_info *info)
{
   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
      { 0 },
   };

   char TMPC[512];

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
      return false;
   }

   snprintf(retro_game_path, sizeof(retro_game_path), "%s", info->path);

   check_variables();

   char buf[256];
   int err = 0;
   if (strstr(info->path, "gbc"))
   {
      GB_init(&gb);
      snprintf(buf, sizeof(buf), "%s%cdmg_boot.bin", retro_base_directory, slash);
      err = GB_load_boot_rom(&gb, buf);

      if (err)
         err = GB_load_boot_rom_dmg(&gb);
   }
   else 
   {
      GB_init_cgb(&gb);
      snprintf(buf, sizeof(buf), "%s%ccgb_boot.bin", retro_base_directory, slash);
      err = GB_load_boot_rom(&gb, buf);

      if (err)
         err = GB_load_boot_rom_cgb(&gb);
   }

   if (err) 
      log_cb(RETRO_LOG_INFO, "Failed to load boot ROM %s %d\n", buf, err);
   (void)info;

   if (GB_load_rom(&gb,info->path)) {
        perror("Failed to load ROM");
        exit(1);
   }

   signal(SIGINT, debugger_interrupt);

    GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    GB_set_user_data(&gb, (void*)NULL);
    GB_set_pixels_output(&gb,(unsigned char *) frame_buf);
    GB_set_rgb_encode_callback(&gb, rgb_encode);

    size_t path_length = strlen(retro_game_path);

    /* Configure battery */
    replace_extension(retro_game_path, path_length, battery_save_path, ".sav");
    GB_load_battery(&gb, battery_save_path);
printf("(%s)\n",battery_save_path);
    /* Configure symbols */

    sprintf(TMPC,"%s/registers.sym",retro_base_directory);
    GB_debugger_load_symbol_file(&gb, TMPC);
    
    replace_extension(retro_game_path, path_length, symbols_path, ".sym");
    GB_debugger_load_symbol_file(&gb, symbols_path);

    GB_set_sample_rate(&gb, AUDIO_FREQUENCY);
   return true;
}

void retro_unload_game(void)
{

}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   return false;
}

size_t retro_serialize_size(void)
{
   return false;
}

bool retro_serialize(void *data_, size_t size)
{
   return false;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

