#include "gb.h"

static size_t GB_map_find_symbol_index(GB_symbol_map_t *map, uint16_t addr)
{
    if (!map->symbols) {
        return 0;
    }
    ssize_t min = 0;
    ssize_t max = map->n_symbols;
    while (min < max) {
        size_t pivot = (min + max) / 2;
        if (map->symbols[pivot].addr == addr) return pivot;
        if (map->symbols[pivot].addr > addr) {
            max = pivot;
        }
        else {
            min = pivot + 1;
        }
    }
    return (size_t) min;
}

void GB_map_add_symbol(GB_symbol_map_t *map, uint16_t addr, const char *name)
{
    size_t index = GB_map_find_symbol_index(map, addr);

    if (index < map->n_symbols && map->symbols[index].addr == addr) return;

    map->symbols = realloc(map->symbols, (map->n_symbols + 1) * sizeof(map->symbols[0]));
    memmove(&map->symbols[index + 1], &map->symbols[index], (map->n_symbols - index) * sizeof(map->symbols[0]));
    map->symbols[index].addr = addr;
    map->symbols[index].name = strdup(name);
    map->n_symbols++;
}

const GB_bank_symbol_t *GB_map_find_symbol(GB_symbol_map_t *map, uint16_t addr)
{
    if (!map) return NULL;
    size_t index = GB_map_find_symbol_index(map, addr);
    if (index < map->n_symbols && map->symbols[index].addr != addr) {
        index--;
    }
    if (index < map->n_symbols) {
        return &map->symbols[index];
    }
    return NULL;
}

GB_symbol_map_t *GB_map_alloc(void)
{
    GB_symbol_map_t *map = malloc(sizeof(*map));
    memset(map, 0, sizeof(*map));
    return map;
}

void GB_map_free(GB_symbol_map_t *map)
{
    for (unsigned char i = 0; i < map->n_symbols; i++) {
        free(map->symbols[i].name);
    }

    if (map->symbols) {
        free(map->symbols);
    }

    free(map);
}