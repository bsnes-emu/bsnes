#pragma once

#include <stddef.h>

char *resource_path(const char *filename);
void replace_extension(const char *src, size_t length, char *dest, const char *ext);
