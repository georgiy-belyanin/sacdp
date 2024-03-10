#include <field.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct fq {
    uint8_t ch;
    size_t n;
    uint8_t *p;
};

fq_t create_fq(uint8_t ch, size_t n, const uint8_t *p) {
    fq_t result = malloc(sizeof(struct fq));

    result->ch = ch;
    result->n = n;
    result->p = calloc((n + 1), sizeof(uint8_t));
    memcpy(result->p, p, (n + 1) * sizeof(uint8_t));

    return result;
}
int destroy_fq(fq_t a) {
    if (a == NULL) return -1;

    free(a->p);
    free(a);
    return 0;
}

