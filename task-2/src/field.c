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
    if (result == NULL) return NULL;

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
    if (fq == NULL) return NULL;

    f_t result = malloc(sizeof(struct f));
    if (result == NULL) return NULL;

    result->fq = fq;
    result->digits = calloc(fq->n, sizeof(uint8_t));
    if (result->digits == NULL) {
        free(result);
        return NULL;
    }

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
    if (result == NULL) return NULL;
    result->digits[0] = 1;

    return result;
}
f_t fq_get_from_array(cfq_t a, const uint8_t *p) {
    if (a == NULL) return NULL;
    if (p == NULL) return NULL;

    f_t result = create_f(a);
    if (result == NULL) return NULL;
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

f_t f_mul(cf_t a, cf_t b) {
    if (a == NULL) return NULL;
    if (b == NULL) return NULL;
    if (!f_same_fq(a, b)) return NULL;

    cfq_t fq = a->fq;

    uint8_t *digits = calloc(2 * fq->n - 1, sizeof(uint8_t));
    if (digits == NULL) return NULL;

    f_t result = create_f(fq);
    if (result == NULL) {
        free(digits);
        return NULL;
    }

    for (size_t i = 0; i < fq->n; i++) {
        for (size_t j = 0; j < fq->n; j++) {
            digits[i + j] = (digits[i + j] + a->digits[i] * b->digits[j]) % fq->ch;
        }
    }

    for (int32_t i = fq->n - 2; i >= 0; i--) {
        int8_t mul = digits[fq->n + i];
        for (int32_t j = fq->n; j >= 0; j--) {
            int32_t digitn = (digits[i + j] - fq->p[j] * mul) % fq->ch;
            digits[i + j] = digitn >= 0 ? digitn : digitn + fq->ch;
        }
    }

    free(result->digits);
    result->digits = realloc(digits, fq->n * sizeof(uint8_t));

    return result;
}

