#include <field.h>

#include <stdbool.h>
#include <stdio.h>
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
    if (p[n] != 1) return NULL;

    result->ch = ch;
    result->n = n;
    result->p = malloc((n + 1) * sizeof(uint8_t));
    if (result->p == NULL) {
        free(result);
        return NULL;
    }
    memcpy(result->p, p, (n + 1) * sizeof(uint8_t));

    return result;
}
void destroy_fq(fq_t a) {
    if (a == NULL) return;

    free(a->p);
    free(a);
}

// Polynomial: x^8 + x^4 + x^3 + x^2 + 1
static uint8_t _f2p8_p[] = { 1, 0, 1, 1, 1, 0, 0, 0, 1 };
static struct fq _f2p8 = { 2, 8, _f2p8_p };
// Polynomial: x^16 + x^12 + x^3 + x^1 + 1
static uint8_t _f2p16_p[] = { 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 };
static struct fq _f2p16 = { 2, 16, _f2p16_p };
// Polynomial: x^32 + x^22 + x^2 + x^1 + 1
static uint8_t _f2p32_p[] = { 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
static struct fq _f2p32 = { 2, 32, _f2p32_p };

const cfq_t F2P8 = &_f2p8;
const cfq_t F2P16 = &_f2p16;
const cfq_t F2P32 = &_f2p32;

struct f {
    cfq_t fq;
    uint8_t *digits;
};

static f_t create_f(cfq_t fq) {
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
void destroy_f(f_t a) {
    if (a == NULL) return;

    free(a->digits);
    free(a);
}

static f_t f_copy(cf_t f) {
    if (f == NULL) return NULL;

    cfq_t fq = f->fq;
    f_t result = create_f(fq);
    if (result == NULL) return NULL;

    for (size_t i = 0; i < fq->n; i++)
        result->digits[i] = f->digits[i];

    return result;
}

static bool f_same_fq(cf_t a, cf_t b) {
    if (a == NULL) return false;
    if (b == NULL) return false;

    if (a->fq->ch != b->fq->ch || a->fq->n != b->fq->n) return false;
    return memcmp(a->fq->p, b->fq->p, a->fq->n + 1) == 0;
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

f_t f_from_uint8(uint8_t a) {
    f_t result = create_f(F2P8);
    if (result == NULL) return NULL;

    for (size_t i = 0; i < 8; i++) {
        result->digits[i] = a & 1;
        a >>= 1;
    }
    return result;
}
f_t f_from_uint16(uint16_t a) {
    f_t result = create_f(F2P16);
    if (result == NULL) return NULL;

    for (size_t i = 0; i < 16; i++) {
        result->digits[i] = a & 1;
        a >>= 1;
    }
    return result;
}
f_t f_from_uint32(uint32_t a) {
    f_t result = create_f(F2P32);
    if (result == NULL) return NULL;

    for (size_t i = 0; i < 32; i++) {
        result->digits[i] = a & 1;
        a >>= 1;
    }
    return result;
}

int f_print(cf_t a) {
    if (a == NULL) return -1;

    cfq_t fq = a->fq;
    for (ssize_t i = fq->n - 1; i >= 0; i--) {
        printf("%dx^%ld", a->digits[i], i);
        if (i != 0) printf(" + ");
    }
    printf("\n");

    return 0;
}

int f_eq(cf_t a, cf_t b) {
    if (a == NULL) return 0;
    if (b == NULL) return 0;
    if (!f_same_fq(a, b)) return 0;

    return memcmp(a->digits, b->digits, a->fq->n) == 0;
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

f_t f_sub(cf_t a, cf_t b) {
    if (a == NULL) return NULL;
    if (b == NULL) return NULL;
    if (!f_same_fq(a, b)) return NULL;

    f_t b_neg = f_neg(b);
    if (b_neg == NULL) return NULL;
    f_t result = f_add(a, b_neg);
    destroy_f(b_neg);

    return result;
}

f_t f_neg(cf_t a) {
    if (a == NULL) return NULL;

    cfq_t fq = a->fq;
    f_t result = create_f(fq);
    if (result == NULL) return NULL;

    for (size_t i = 0; i < fq->n; i++) {
        result->digits[i] = (fq->ch - a->digits[i]) % fq->ch;
    }

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

    for (ssize_t i = fq->n - 2; i >= 0; i--) {
        int8_t mul = digits[fq->n + i];
        for (ssize_t j = fq->n; j >= 0; j--) {
            int32_t digitn = (digits[i + j] - fq->p[j] * mul) % fq->ch;
            digits[i + j] = digitn >= 0 ? digitn : digitn + fq->ch;
        }
    }

    memcpy(result->digits, digits, fq->n * sizeof(*(result->digits)));
    free(digits);

    return result;
}

static uint64_t uint_pow(uint64_t base, uint64_t exp) {
    uint64_t result = 1;
    while (exp) {
        if (exp % 2 == 1) {
            result *= base;
            exp--;
        } else {
            exp /= 2;
            base *= base;
        }
    }
    return result;
}

static f_t f_pow(cf_t a, uint64_t exp) {
    cfq_t fq = a->fq;
    f_t result = fq_get_identity(fq);
    if (result == NULL) return NULL;
    f_t base = f_copy(a);
    if (base == NULL) {
        destroy_f(result);
        return NULL;
    }

    while (exp) {
        if (exp % 2 == 1) {
            f_t resultn = f_mul(result, base);
            if (resultn == NULL) {
                destroy_f(result);
                destroy_f(base);
                return NULL;
            }
            destroy_f(result);
            result = resultn;
            exp--;
        } else {
            f_t basen = f_mul(base, base);
            if (basen == NULL) {
                destroy_f(result);
                destroy_f(base);
                return NULL;
            }
            destroy_f(base);
            base = basen;
            exp /= 2;
        }
    }
    destroy_f(base);
    return result;
}

f_t f_inv(cf_t a) {
    if (a == NULL) return NULL;

    cfq_t fq = a->fq;
    size_t d = uint_pow(fq->ch, fq->n) - 2;
    return f_pow(a, d);
}

f_t f_div(cf_t a, cf_t b) {
    if (a == NULL) return NULL;
    if (b == NULL) return NULL;

    f_t b_inv = f_inv(b);
    if (b_inv == NULL) return NULL;
    f_t result = f_mul(a, b_inv);
    destroy_f(b_inv);

    return result;
}
