#pragma once

#include <stddef.h>
#include <stdint.h>

struct fq;
typedef struct fq *fq_t;
typedef const struct fq *cfq_t;

struct f;
typedef struct f *f_t;
typedef const struct f *cf_t;

fq_t create_fq(uint8_t ch, size_t n, const uint8_t *p);
void destroy_fq(fq_t a);

f_t fq_get_zero(cfq_t a);
f_t fq_get_identity(cfq_t a);
f_t fq_get_from_array(cfq_t a, const uint8_t *p);

extern const cfq_t F2P8;
extern const cfq_t F2P16;
extern const cfq_t F2P32;

f_t f_from_uint8(uint8_t a);
f_t f_from_uint16(uint16_t a);
f_t f_from_uint32(uint32_t a);

void destroy_f(f_t a);

int f_print(cf_t a);

int f_eq(cf_t a, cf_t b);

f_t f_add(cf_t a, cf_t b);
f_t f_sub(cf_t a, cf_t b);
f_t f_neg(cf_t a);
f_t f_mul(cf_t a, cf_t b);
f_t f_inv(cf_t a);
f_t f_div(cf_t a, cf_t b);

