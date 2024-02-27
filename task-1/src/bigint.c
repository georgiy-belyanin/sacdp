#include "bigint.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define BASE 10

enum sign {
    PLUS = 0,
    MINUS = 1
};

struct dec {
    size_t length;
    enum sign sign;
    int8_t *digits;
};

static dec_t create_dec(size_t length, enum sign sign) {
    dec_t result = malloc(sizeof(struct dec));
    result->length = length;
    result->sign = sign;
    result->digits = calloc(length, sizeof(int8_t));
    return result;
}

void destroy_dec(dec_t a) {
    free(a->digits);
    free(a);
}

static int dec_truncate(dec_t a) {
    if (a == NULL)    
        return -EINVAL;

    size_t leading_zeros = 0;
    while(leading_zeros < a->length && a->digits[a->length - leading_zeros - 1] == 0)
        leading_zeros++;

    if (leading_zeros == 0)
        return 0;

    size_t new_length = max(a->length - leading_zeros, 1);
    a->digits = realloc(a->digits, new_length * sizeof(int8_t));
    if (a->digits == NULL)
        return -ENOMEM;
    a->length = new_length;

    return 0;
}
static int dec_extend(dec_t a, size_t n) {
    if (a == NULL)
        return -EINVAL;

    a->digits = realloc(a->digits, a->length + n);
    if (a->digits == NULL)
        return -ENOMEM;
    for (size_t i = a->length; i < a->length + n; i++)
        a->digits[i] = 0;

    a->length += n;

    return 0;
}
static dec_t dec_copy(cdec_t a) {
    dec_t result = create_dec(a->length, a->sign);
    for (size_t i = 0; i < a->length; i++)
        result->digits[i] = a->digits[i];
    return result;
}

#if (BASE == 10)
dec_t dec_from_string(const char* s) {
    if (s == NULL)
        return NULL;

    bool negative = s[0] == '-'; 
    size_t len = strlen(s);

    for (size_t i = 0; i < len - negative; i++)
        if (s[len - i - 1] - '0' > 10 || s[len - i - 1] - '0' < 0)
            return NULL;

    dec_t result = create_dec(len - negative, negative);
    if (result == NULL)
        return NULL;

    for (size_t i = 0; i < len - negative; i++)
        result->digits[i] = s[len - i - 1] - '0';
    
    dec_truncate(result);
    return result;
}
#else
#warning dec_from_string() unimplemented

dec_t dec_from_string(const char* s) {
    return NULL;
}
#endif

dec_t dec_from_int(int64_t value) {
    dec_t result = create_dec(20, value < 0);
    
    int i = 0;
    for (value = llabs(value); value > 0; value /= BASE) {
        result->digits[i++] = value % BASE;
    }

    dec_truncate(result);
    return result;
}

char *dec_to_string(cdec_t a) {
    if (a == NULL)
        return NULL;

    if (a->length == 1 && a->digits[0] == 0) {
        char *result = calloc(2, sizeof(char));
        result[0] = '0';
        return result;
    }

    size_t len = a->length + a->sign;
    char *result = calloc(len + 1, sizeof(char));
    if (result == NULL)
        return NULL;

    if (a->sign == MINUS)
        result[0] = '-';

    for (int i = 0; i < a->length; i++)
        result[len - i - 1] = '0' + a->digits[i];

    return result;
}

dec_t dec_add(cdec_t a, cdec_t b) {
    if (a == NULL || b == NULL)
        return NULL;

    size_t max_length = max(a->length, b->length);
    dec_t result = create_dec(max_length, a->sign);

    if (result == NULL)
        return NULL;

    if (a->sign != b->sign) {
        for (size_t i = 0; i < max_length; i++) {
            int8_t a_digit = (i < a->length) ? a->digits[i] : 0;
            int8_t b_digit = (i < b->length) ? b->digits[i] : 0;

            result->digits[i] += a_digit - b_digit;
            if (result->digits[i] < 0) {
                result->digits[i] += 10;

                if (i != max_length - 1)
                    result->digits[i+1]--;
                else
                    result->sign = 1 - result->sign;
            }
        }
        dec_truncate(result);
    } else {
        result->sign = a->sign;
        for (size_t i = 0; i < max_length; i++) {
            int8_t a_digit = (i < a->length) ? a->digits[i] : 0;
            int8_t b_digit = (i < b->length) ? b->digits[i] : 0;

            result->digits[i] += a_digit + b_digit;
            if (result->digits[i] >= BASE) {
                result->digits[i] %= BASE;

                if (i == max_length - 1)
                    if (dec_extend(result, 1) < 0)
                        return NULL;
                result->digits[i+1]++;
            }
        }
    }

    return result;
}

dec_t dec_sub(cdec_t a, cdec_t b) {
    if (a == NULL || b == NULL)
        return NULL;

    dec_t neg_b = dec_copy(b);
    neg_b->sign = 1 - neg_b->sign;

    dec_t result = dec_add(a, neg_b);
    if (result == NULL)
        return NULL;

    destroy_dec(neg_b);
    return result;
}

dec_t dec_mul(cdec_t a, cdec_t b) {
    if (a == NULL || b == NULL)
        return NULL;

    size_t length = a->length + b->length;
    dec_t result = create_dec(length, (a->sign + b->sign) % 2);

    if (result == NULL)
        return NULL;

    for (size_t i = 0; i < b->length; i++) {
        int8_t b_digit = b->digits[i];
        for (size_t j = 0; j < a->length; j++) {
            int8_t a_digit = a->digits[i];

            result->digits[i + j] += a_digit * b_digit;
            if (result->digits[i + j] >= 10) {
                result->digits[i + j + 1] += result->digits[i + j] / 10;
                result->digits[i + j] %= 10;
            }
        }
    }
    dec_truncate(result);

    return result;
}

int8_t dec_cmp(cdec_t a, cdec_t b) {
    if (a->length == 1 && b->length == 1 && a->digits[0] == 0 && b->digits[0] == 0)
        return 0;
    else if (a->sign > b->sign)
        return -1;
    else if (b->sign > a->sign)
        return 1;
    else if (a->length > b->length)
        return 1 * (a->sign * 2 - 1);
    else if (b->length > a->length)
        return -1 * (a->sign * 2 - 1);
    else {
        for (int32_t i = a->length - 1; i >= 0; i--) {
            if (a->digits[i] < b->digits[i])
                return 1 * (a->sign * 2 - 1);
            else if (b->digits[i] < a->digits[i])
                return -1 * (a->sign * 2 - 1);
        }
        return 0;
    }
}
