#include <stddef.h>
#include <stdint.h>

struct fq;
typedef struct fq *fq_t;
typedef const struct fq *cfq_t;

struct f;
typedef struct f *f_t;
typedef const struct f *cf_t;

fq_t create_fq(uint8_t ch, size_t n, const uint8_t *p);
int destroy_fq(fq_t a);

