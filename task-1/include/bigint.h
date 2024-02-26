#pragma once

#include <stdint.h>

struct dec;
typedef struct dec *dec_t;
typedef const struct dec *cdec_t;

void destroy_dec(dec_t dec);

dec_t dec_from_string(const char* string);
dec_t dec_from_int(int64_t value);

char *dec_to_string();

dec_t dec_add(cdec_t a, cdec_t b);
dec_t dec_sub(cdec_t a, cdec_t b);
dec_t dec_mul(cdec_t a, cdec_t b);

