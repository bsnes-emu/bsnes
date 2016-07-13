#ifndef symbol_hash_h
#define symbol_hash_h

#include <stdlib.h>
#include <string.h>

typedef struct {
    uint16_t addr;
    char *name;
} GB_bank_symbol_t;


typedef struct {
    GB_bank_symbol_t *symbols;
    size_t n_symbols;
} GB_symbol_map_t;

void GB_map_add_symbol(GB_symbol_map_t *map, uint16_t addr, const char *name);
const GB_bank_symbol_t *GB_map_find_symbol(GB_symbol_map_t *map, uint16_t addr);
GB_symbol_map_t *GB_map_alloc(void);
void GB_map_free(GB_symbol_map_t *map);

#endif /* symbol_hash_h */
