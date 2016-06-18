/* Macros to make the GB_gameboy_t struct more future compatible when state saving */
#ifndef save_struct_h
#define save_struct_h

#define GB_PADDING(type, old_usage) type old_usage##__do_not_use

#define GB_SECTION(name, ...) __attribute__ ((aligned (8))) struct {} name##_section_start; __VA_ARGS__; struct {} name##_section_end
#define GB_SECTION_OFFSET(name) (offsetof(GB_gameboy_t, name##_section_start))
#define GB_SECTION_SIZE(name) (offsetof(GB_gameboy_t, name##_section_end) - offsetof(GB_gameboy_t, name##_section_start))
#define GB_GET_SECTION(gb, name) ((void*)&((gb)->name##_section_start))

#define GB_aligned_double __attribute__ ((aligned (8))) double

#endif /* save_struct_h */
