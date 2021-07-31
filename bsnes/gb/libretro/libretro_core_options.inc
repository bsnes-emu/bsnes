#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include "libretro.h"
#include "retro_inline.h"

/*
 ********************************
 * VERSION: 1.3
 ********************************
 *
 * - 1.3: Move translations to libretro_core_options_intl.h
 *        - libretro_core_options_intl.h includes BOM and utf-8
 *          fix for MSVC 2010-2013
 *        - Added HAVE_NO_LANGEXTRA flag to disable translations
 *          on platforms/compilers without BOM support
 * - 1.2: Use core options v1 interface when
 *        RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION is >= 1
 *        (previously required RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION == 1)
 * - 1.1: Support generation of core options v0 retro_core_option_value
 *        arrays containing options with a single value
 * - 1.0: First commit
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 ********************************
 * Core Option Definitions
 ********************************
 */

/* RETRO_LANGUAGE_ENGLISH */

/* Default language:
 * - All other languages must include the same keys and values
 * - Will be used as a fallback in the event that frontend language
 *   is not available
 * - Will be used as a fallback for any missing entries in
 *   frontend language definition */

struct retro_core_option_definition option_defs_us[] = {
    
    /* Core options used in single cart mode */
    
    {
        "sameboy_model",
        "System - Emulated Model (Requires Restart)",
        "Chooses which system model the content should be started on. Certain games may activate special in-game features when ran on specific models. This option requires a content restart in order to take effect.",
        {
            { "Auto",                 "Detect automatically"                      },
            { "Game Boy",             "Game Boy (DMG-CPU B)"                      },
            { "Game Boy Color C",     "Game Boy Color (CPU-CGB C) (Experimental)" },
            { "Game Boy Color",       "Game Boy Color (CPU-CGB E)"                },
            { "Game Boy Advance",     NULL },
            { "Super Game Boy",       "Super Game Boy NTSC"                       },
            { "Super Game Boy PAL",   NULL },
            { "Super Game Boy 2",     NULL },
            { NULL, NULL },
        },
        "Auto"
    },
    {
        "sameboy_rtc",
        "System - Real Time Clock Emulation",
        "Specifies how the emulation of the real-time clock feature used in certain Game Boy and Game Boy Color games should function.",
        {
            { "sync to system clock", "Sync to System Clock" },
            { "accurate",             "Accurate"             },
            { NULL, NULL },
        },
        "sync to system clock"
    },
    
    {
        "sameboy_mono_palette",
        "Video - GB Mono Palette",
        "Selects the color palette that should be used when playing Game Boy games.",
        {
            { "greyscale", "Greyscale"               },
            { "lime",      "Lime (Game Boy)"         },
            { "olive",     "Olive (Game Boy Pocket)" },
            { "teal",      "Teal (Game Boy Light)"   },
            { NULL, NULL },
        },
        "greyscale"
    },
    {
        "sameboy_color_correction_mode",
        "Video - GBC Color Correction",
        "Defines which type of color correction should be applied when playing Game Boy Color games.",
        {
            { "emulate hardware",    "Emulate Hardware"             },
            { "preserve brightness", "Preserve Brightness"          },
            { "reduce contrast",     "Reduce Contrast"              },
            { "correct curves",      "Correct Color Curves"         },
            { "harsh reality",       "Harsh Reality (Low Contrast)" },
            { "off",                 "Disabled"                     },
            { NULL, NULL },
        },
        "emulate hardware"
    },
    {
        "sameboy_light_temperature",
        "Video - Ambient Light Temperature",
        "Simulates an ambient light’s effect on non-backlit Game Boy screens, by setting a user-controlled color temperature. This option has no effect if the content is run on an original Game Boy (DMG) emulated model.",
        {
            { "1.0",   "1000K (Warmest)"       },
            { "0.9",   "1550K" },
            { "0.8",   "2100K" },
            { "0.7",   "2650K" },
            { "0.6",   "3200K" },
            { "0.5",   "3750K" },
            { "0.4",   "4300K" },
            { "0.3",   "4850K" },
            { "0.2",   "5400K" },
            { "0.1",   "5950K" },
            { "0",     "6500K (Neutral White)" },
            { "-0.1",  "7050K" },
            { "-0.2",  "7600K" },
            { "-0.3",  "8150K" },
            { "-0.4",  "8700K" },
            { "-0.5",  "9250K" },
            { "-0.6",  "9800K" },
            { "-0.7", "10350K" },
            { "-0.8", "10900K" },
            { "-0.9", "11450K" },
            { "-1.0", "12000K (Coolest)"      },
            { NULL, NULL },
        },
        "0"
    },
    {
        "sameboy_border",
        "Video - Display Border",
        "Defines when to display an on-screen border around the content.",
        {
            { "always",              "Always"                  },
            { "Super Game Boy only", "Only for Super Game Boy" },
            { "never",               "Disabled"                },
            { NULL, NULL },
        },
        "Super Game Boy only"
    },
    {
        "sameboy_high_pass_filter_mode",
        "Audio - Highpass Filter",
        "Applies a filter to the audio output, removing certain pop sounds caused by the DC Offset. If disabled, the sound will be rendered as output by the Game Boy APU, but popping effects will be heard when the emulator is paused or resumed. 'Accurate' will apply a global filter, masking popping sounds while also reducing lower frequencies. 'Preserve Waveform' applies the filter only to the DC Offset.",
        {
            { "accurate",         "Accurate"              },
            { "remove dc offset", "Preserve Waveform"     },
            { "off",              "Disabled"              },
            { NULL, NULL },
        },
        "accurate"
    },
    {
        "sameboy_audio_interference",
        "Audio - Interference Volume",
        "Controls the volume of the buzzing effect caused by the electrical interference between the Game Boy SoC and the system speakers.",
        {
            { "0",   "0%"   },
            { "5",   "5%"   },
            { "10",  "10%"  },
            { "15",  "15%"  },
            { "20",  "20%"  },
            { "25",  "25%"  },
            { "30",  "30%"  },
            { "35",  "35%"  },
            { "40",  "40%"  },
            { "45",  "45%"  },
            { "50",  "50%"  },
            { "55",  "55%"  },
            { "60",  "60%"  },
            { "65",  "65%"  },
            { "70",  "70%"  },
            { "75",  "75%"  },
            { "80",  "80%"  },
            { "85",  "85%"  },
            { "90",  "90%"  },
            { "95",  "95%"  },
            { "100", "100%" },
            { NULL, NULL },
        },
        "0"
    },
    {
        "sameboy_rumble",
        "Input - Rumble Mode",
        "Defines which type of content should trigger rumble effects.",
        {
            { "all games",            "Always"                        },
            { "rumble-enabled games", "Only for rumble-enabled games" },
            { "never",                "Never"                      },
            { NULL, NULL },
        },
        "rumble-enabled games"
    },
    
    /* Core options used in dual cart mode */
    
    {
        "sameboy_link",
        "System - Link Cable Emulation",
        "Enables the emulation of the link cable, allowing communication and exchange of data between two Game Boy systems.",
        {
            { "enabled",  "Enabled" },
            { "disabled", "Disabled" },
            { NULL, NULL },
        },
        "enabled"
    },
    {
        "sameboy_screen_layout",
        "System - Screen Layout",
        "When emulating two systems at once, this option defines the respective position of the two screens.",
        {
            { "top-down",   "Top-Down"   },
            { "left-right", "Left-Right" },
            { NULL, NULL },
        },
        "top-down"
    },
    {
        "sameboy_audio_output",
        "System - Audio Output",
        "Selects which of the two emulated Game Boy systems should output audio.",
        {
            { "Game Boy #1", NULL },
            { "Game Boy #2", NULL },
            { NULL, NULL },
        },
        "Game Boy #1"
    },
    {
        "sameboy_model_1",
        "System - Emulated Model for Game Boy #1 (Requires Restart)",
        "Chooses which system model the content should be started on for Game Boy #1. Certain games may activate special in-game features when ran on specific models. This option requires a content restart in order to take effect.",
        {
            { "Auto",                 "Detect automatically"                      },
            { "Game Boy",             "Game Boy (DMG-CPU B)"                      },
            { "Game Boy Color C",     "Game Boy Color (CPU-CGB C) (Experimental)" },
            { "Game Boy Color",       "Game Boy Color (CPU-CGB E)"                },
            { "Game Boy Advance",     NULL },
            { "Super Game Boy",       "Super Game Boy NTSC"                       },
            { "Super Game Boy PAL",   NULL },
            { "Super Game Boy 2",     NULL },
            { NULL, NULL },
        },
        "Auto"
    },
    {
        "sameboy_model_2",
        "System - Emulated Model for Game Boy #2 (Requires Restart)",
        "Chooses which system model the content should be started on for Game Boy #2. Certain games may activate special in-game features when ran on specific models. This option requires a content restart in order to take effect.",
        {
            { "Auto",                 "Detect automatically"                      },
            { "Game Boy",             "Game Boy (DMG-CPU B)"                      },
            { "Game Boy Color C",     "Game Boy Color (CPU-CGB C) (Experimental)" },
            { "Game Boy Color",       "Game Boy Color (CPU-CGB E)"                },
            { "Game Boy Advance",     NULL },
            { "Super Game Boy",       "Super Game Boy NTSC"                       },
            { "Super Game Boy PAL",   NULL },
            { "Super Game Boy 2",     NULL },
            { NULL, NULL },
        },
        "Auto"
    },
    {
        "sameboy_mono_palette_1",
        "Video - GB Mono Palette for Game Boy #1",
        "Selects the color palette that should be used when playing Game Boy games on Game Boy #1.",
        {
            { "greyscale", "Greyscale"               },
            { "lime",      "Lime (Game Boy)"         },
            { "olive",     "Olive (Game Boy Pocket)" },
            { "teal",      "Teal (Game Boy Light)"   },
            { NULL, NULL },
        },
        "greyscale"
    },
    {
        "sameboy_mono_palette_2",
        "Video - GB Mono Palette for Game Boy #2",
        "Selects the color palette that should be used when playing Game Boy games on Game Boy #2.",
        {
            { "greyscale", "Greyscale"               },
            { "lime",      "Lime (Game Boy)"         },
            { "olive",     "Olive (Game Boy Pocket)" },
            { "teal",      "Teal (Game Boy Light)"   },
            { NULL, NULL },
        },
        "greyscale"
    },
    {
        "sameboy_color_correction_mode_1",
        "Video - GBC Color Correction for Game Boy #1",
        "Defines which type of color correction should be applied when playing Game Boy Color games on Game Boy #1.",
        {
            { "emulate hardware",    "Emulate Hardware"             },
            { "preserve brightness", "Preserve Brightness"          },
            { "reduce contrast",     "Reduce Contrast"              },
            { "correct curves",      "Correct Color Curves"         },
            { "harsh reality",       "Harsh Reality (Low Contrast)" },
            { "off",                 "Disabled"                     },
            { NULL, NULL },
        },
        "emulate hardware"
    },
    {
        "sameboy_color_correction_mode_2",
        "Video - GBC Color Correction for Game Boy #2",
        "Defines which type of color correction should be applied when playing Game Boy Color games on Game Boy #2.",
        {
            { "emulate hardware",    "Emulate Hardware"             },
            { "preserve brightness", "Preserve Brightness"          },
            { "reduce contrast",     "Reduce Contrast"              },
            { "correct curves",      "Correct Color Curves"         },
            { "harsh reality",       "Harsh Reality (Low Contrast)" },
            { "off",                 "Disabled"                     },
            { NULL, NULL },
        },
        "emulate hardware"
    },
    {
        "sameboy_light_temperature_1",
        "Video - Ambient Light Temperature for Game Boy #1",
        "Simulates an ambient light’s effect on non-backlit Game Boy screens, by setting a user-controlled color temperature applied to the screen of Game Boy #1. This option has no effect if the content is run on an original Game Boy (DMG) emulated model.",
        {
            { "1.0",   "1000K (Warmest)"       },
            { "0.9",   "1550K" },
            { "0.8",   "2100K" },
            { "0.7",   "2650K" },
            { "0.6",   "3200K" },
            { "0.5",   "3750K" },
            { "0.4",   "4300K" },
            { "0.3",   "4850K" },
            { "0.2",   "5400K" },
            { "0.1",   "5950K" },
            { "0",     "6500K (Neutral White)" },
            { "-0.1",  "7050K" },
            { "-0.2",  "7600K" },
            { "-0.3",  "8150K" },
            { "-0.4",  "8700K" },
            { "-0.5",  "9250K" },
            { "-0.6",  "9800K" },
            { "-0.7", "10350K" },
            { "-0.8", "10900K" },
            { "-0.9", "11450K" },
            { "-1.0", "12000K (Coolest)"      },
            { NULL, NULL },
        },
        "0"
    },
    {
        "sameboy_light_temperature_2",
        "Video - Ambient Light Temperature for Game Boy #2",
        "Simulates an ambient light’s effect on non-backlit Game Boy screens, by setting a user-controlled color temperature applied to the screen of Game Boy #2. This option has no effect if the content is run on an original Game Boy (DMG) emulated model.",
        {
            { "1.0",   "1000K (Warmest)"       },
            { "0.9",   "1550K" },
            { "0.8",   "2100K" },
            { "0.7",   "2650K" },
            { "0.6",   "3200K" },
            { "0.5",   "3750K" },
            { "0.4",   "4300K" },
            { "0.3",   "4850K" },
            { "0.2",   "5400K" },
            { "0.1",   "5950K" },
            { "0",     "6500K (Neutral White)" },
            { "-0.1",  "7050K" },
            { "-0.2",  "7600K" },
            { "-0.3",  "8150K" },
            { "-0.4",  "8700K" },
            { "-0.5",  "9250K" },
            { "-0.6",  "9800K" },
            { "-0.7", "10350K" },
            { "-0.8", "10900K" },
            { "-0.9", "11450K" },
            { "-1.0", "12000K (Coolest)"      },
            { NULL, NULL },
        },
        "0"
    },
    {
        "sameboy_high_pass_filter_mode_1",
        "Audio - Highpass Filter for Game Boy #1",
        "Applies a filter to the audio output for Game Boy #1, removing certain pop sounds caused by the DC Offset. If disabled, the sound will be rendered as output by the Game Boy APU, but popping effects will be heard when the emulator is paused or resumed. 'Accurate' will apply a global filter, masking popping sounds while also reducing lower frequencies. 'Preserve Waveform' applies the filter only to the DC Offset.",
        {
            { "accurate",         "Accurate"              },
            { "remove dc offset", "Preserve Waveform"     },
            { "off",              "disabled"              },
            { NULL, NULL },
        },
        "accurate"
    },
    {
        "sameboy_high_pass_filter_mode_2",
        "Audio - Highpass Filter for Game Boy #2",
        "Applies a filter to the audio output for Game Boy #2, removing certain pop sounds caused by the DC Offset. If disabled, the sound will be rendered as output by the Game Boy APU, but popping effects will be heard when the emulator is paused or resumed. 'Accurate' will apply a global filter, masking popping sounds while also reducing lower frequencies. 'Preserve Waveform' applies the filter only to the DC Offset.",
        {
            { "accurate",         "Accurate"              },
            { "remove dc offset", "Preserve Waveform"     },
            { "off",              "disabled"              },
            { NULL, NULL },
        },
        "accurate"
    },
    {
        "sameboy_audio_interference_1",
        "Audio - Interference Volume for Game Boy #1",
        "Controls the volume of the buzzing effect caused by the electrical interference between the Game Boy SoC and the system speakers on Game Boy #1.",
        {
            { "0",   "0%"   },
            { "5",   "5%"   },
            { "10",  "10%"  },
            { "15",  "15%"  },
            { "20",  "20%"  },
            { "25",  "25%"  },
            { "30",  "30%"  },
            { "35",  "35%"  },
            { "40",  "40%"  },
            { "45",  "45%"  },
            { "50",  "50%"  },
            { "55",  "55%"  },
            { "60",  "60%"  },
            { "65",  "65%"  },
            { "70",  "70%"  },
            { "75",  "75%"  },
            { "80",  "80%"  },
            { "85",  "85%"  },
            { "90",  "90%"  },
            { "95",  "95%"  },
            { "100", "100%" },
            { NULL, NULL },
        },
        "0"
    },
    {
        "sameboy_audio_interference_2",
        "Audio - Interference Volume for Game Boy #2",
        "Controls the volume of the buzzing effect caused by the electrical interference between the Game Boy SoC and the system speakers on Game Boy #2.",
        {
            { "0",   "0%"   },
            { "5",   "5%"   },
            { "10",  "10%"  },
            { "15",  "15%"  },
            { "20",  "20%"  },
            { "25",  "25%"  },
            { "30",  "30%"  },
            { "35",  "35%"  },
            { "40",  "40%"  },
            { "45",  "45%"  },
            { "50",  "50%"  },
            { "55",  "55%"  },
            { "60",  "60%"  },
            { "65",  "65%"  },
            { "70",  "70%"  },
            { "75",  "75%"  },
            { "80",  "80%"  },
            { "85",  "85%"  },
            { "90",  "90%"  },
            { "95",  "95%"  },
            { "100", "100%" },
            { NULL, NULL },
        },
        "0"
    },
    {
        "sameboy_rumble_1",
        "Input - Rumble Mode for Game Boy #1",
        "Defines which type of content should trigger rumble effects when played on Game Boy #1.",
        {
            { "all games",            "Always"                        },
            { "rumble-enabled games", "Only for rumble-enabled games" },
            { "never",                "Never"                      },
            { NULL, NULL },
        },
        "rumble-enabled games"
    },
    {
        "sameboy_rumble_2",
        "Input - Rumble Mode for Game Boy #2",
        "Defines which type of content should trigger rumble effects when played on Game Boy #2.",
        {
            { "all games",            "Always"                        },
            { "rumble-enabled games", "Only for rumble-enabled games" },
            { "never",                "Never"                         },
            { NULL, NULL },
        },
        "rumble-enabled games"
    },
    
    { NULL, NULL, NULL, {{0}}, NULL },
};

/*
 ********************************
 * Language Mapping
 ********************************
 */

#ifndef HAVE_NO_LANGEXTRA
struct retro_core_option_definition *option_defs_intl[RETRO_LANGUAGE_LAST] = {
    option_defs_us, /* RETRO_LANGUAGE_ENGLISH */
    NULL,           /* RETRO_LANGUAGE_JAPANESE */
    NULL,           /* RETRO_LANGUAGE_FRENCH */
    NULL,           /* RETRO_LANGUAGE_SPANISH */
    NULL,           /* RETRO_LANGUAGE_GERMAN */
    NULL,           /* RETRO_LANGUAGE_ITALIAN */
    NULL,           /* RETRO_LANGUAGE_DUTCH */
    NULL,           /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
    NULL,           /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
    NULL,           /* RETRO_LANGUAGE_RUSSIAN */
    NULL,           /* RETRO_LANGUAGE_KOREAN */
    NULL,           /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
    NULL,           /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
    NULL,           /* RETRO_LANGUAGE_ESPERANTO */
    NULL,           /* RETRO_LANGUAGE_POLISH */
    NULL,           /* RETRO_LANGUAGE_VIETNAMESE */
    NULL,           /* RETRO_LANGUAGE_ARABIC */
    NULL,           /* RETRO_LANGUAGE_GREEK */
    NULL,           /* RETRO_LANGUAGE_TURKISH */
    NULL,           /* RETRO_LANGUAGE_SLOVAK */
    NULL,           /* RETRO_LANGUAGE_PERSIAN */
    NULL,           /* RETRO_LANGUAGE_HEBREW */
    NULL,           /* RETRO_LANGUAGE_ASTURIAN */
    NULL,           /* RETRO_LANGUAGE_FINNISH */
    
};
#endif

/*
 ********************************
 * Functions
 ********************************
 */

/* Handles configuration/setting of core options.
 * Should be called as early as possible - ideally inside
 * retro_set_environment(), and no later than retro_load_game()
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb)
{
    unsigned version = 0;
    
    if (!environ_cb) return;
    
    if (environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version) && (version >= 1)) {
#ifndef HAVE_NO_LANGEXTRA
        struct retro_core_options_intl core_options_intl;
        unsigned language = 0;
        
        core_options_intl.us    = option_defs_us;
        core_options_intl.local = NULL;
        
        if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
            (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
            core_options_intl.local = option_defs_intl[language];
        
        environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_intl);
#else
        environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, &option_defs_us);
#endif
    }
    else {
        size_t i;
        size_t num_options               = 0;
        struct retro_variable *variables = NULL;
        char **values_buf                = NULL;
        
        /* Determine number of options */
        while (true) {
            if (!option_defs_us[num_options].key) break;
            num_options++;
        }
        
        /* Allocate arrays */
        variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
        values_buf = (char **)calloc(num_options, sizeof(char *));
        
        if (!variables || !values_buf) goto error;
        
        /* Copy parameters from option_defs_us array */
        for (i = 0; i < num_options; i++) {
            const char *key                        = option_defs_us[i].key;
            const char *desc                       = option_defs_us[i].desc;
            const char *default_value              = option_defs_us[i].default_value;
            struct retro_core_option_value *values = option_defs_us[i].values;
            size_t buf_len                         = 3;
            size_t default_index                   = 0;
            
            values_buf[i] = NULL;
            
            if (desc) {
                size_t num_values = 0;
                
                /* Determine number of values */
                while (true) {
                    if (!values[num_values].value) break;
                    
                    /* Check if this is the default value */
                    if (default_value) {
                        if (strcmp(values[num_values].value, default_value) == 0) default_index = num_values;
                    }
                    
                    buf_len += strlen(values[num_values].value);
                    num_values++;
                }
                
                /* Build values string */
                if (num_values > 0) {
                    size_t j;
                    
                    buf_len += num_values - 1;
                    buf_len += strlen(desc);
                    
                    values_buf[i] = (char *)calloc(buf_len, sizeof(char));
                    if (!values_buf[i]) goto error;
                    
                    strcpy(values_buf[i], desc);
                    strcat(values_buf[i], "; ");
                    
                    /* Default value goes first */
                    strcat(values_buf[i], values[default_index].value);
                    
                    /* Add remaining values */
                    for (j = 0; j < num_values; j++) {
                        if (j != default_index) {
                            strcat(values_buf[i], "|");
                            strcat(values_buf[i], values[j].value);
                        }
                    }
                }
            }
            
            variables[i].key   = key;
            variables[i].value = values_buf[i];
        }
        
        /* Set variables */
        environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
        
    error:
        
        /* Clean up */
        if (values_buf) {
            for (i = 0; i < num_options; i++) {
                if (values_buf[i]) {
                    free(values_buf[i]);
                    values_buf[i] = NULL;
                }
            }
            
            free(values_buf);
            values_buf = NULL;
        }
        
        if (variables) {
            free(variables);
            variables = NULL;
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif
