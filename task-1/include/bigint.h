#pragma once

#include <stdint.h>

struct dec;
typedef struct dec *dec_t;
typedef const struct dec *cdec_t;

dec_t dec_from_string(const char* string);
dec_t dec_from_int(int64_t value);

char *dec_to_string();

