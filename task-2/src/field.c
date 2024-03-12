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

struct f {
    cfq_t fq;
    uint8_t *digits;
};

f_t create_f(cfq_t fq) {
    f_t result = malloc(sizeof(struct f));
    result->fq = fq;
    result->digits = calloc(fq->n, sizeof(uint8_t));
    return result;
}
int destroy_f(f_t a) {
    if (a == NULL) return -1;

    free(a->digits);
    free(a);

    return 0;
}

static bool f_same_fq(cf_t a, cf_t b) {
    if (a == NULL) return false;
    if (b == NULL) return false;

    return a->fq->ch == b->fq->ch && a->fq->n == b->fq->n;
}

f_t fq_get_zero(cfq_t a) {
    if (a == NULL) return NULL;

    return create_f(a);
}

f_t fq_get_identity(cfq_t a) {
    if (a == NULL) return NULL;

    f_t result = create_f(a);
    result->digits[0] = 1;

    return result;
}
f_t fq_get_from_array(cfq_t a, const uint8_t *p) {
    if (a == NULL) return NULL;
    if (p == NULL) return NULL;

    f_t result = create_f(a);
    for (size_t i = 0; i < a->n; i++)
        result->digits[i] = p[i];

    return result;
}

int f_eq(cf_t a, cf_t b) {
    if (a == NULL) return 0;
    if (b == NULL) return 0;
    if (!f_same_fq(a, b)) return 0;

    cfq_t fq = a->fq;
    for (size_t i = 0; i < fq->n; i++)
        if (a->digits[i] != b->digits[i]) return 0;
    return 1;
}

f_t f_add(cf_t a, cf_t b) {
    if (a == NULL) return NULL;
    if (b == NULL) return NULL;
    if (!f_same_fq(a, b)) return NULL;

    cfq_t fq = a->fq;
    f_t result = create_f(fq);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < fq->n; i++)
        result->digits[i] = (a->digits[i] + b->digits[i]) % fq->ch;

    return result;
}

