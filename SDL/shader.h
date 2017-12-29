#ifndef shader_h
#define shader_h
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>

typedef struct shader_s {
    GLuint resolution_uniform;
    GLuint origin_uniform;
    GLuint texture_uniform;
    GLuint previous_texture_uniform;
    GLuint mix_previous_uniform;
    
    GLuint position_attribute;
    GLuint texture;
    GLuint previous_texture;
    GLuint program;
} shader_t;

bool init_shader_with_name(shader_t *shader, const char *name);
void render_bitmap_with_shader(shader_t *shader, void *bitmap, void *previous,unsigned x, unsigned y, unsigned w, unsigned h);
void free_shader(struct shader_s *shader);

#endif /* shader_h */
