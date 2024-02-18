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
    while(a->digits[a->length - leading_zeros - 1] == 0 && leading_zeros < a->length)
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
    if (a->digits)
        return -ENOMEM;

    return 0;
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
        char *result = calloc(1, sizeof(char));
        result[0] = '0';
        return result;
    }

    size_t len = a->length + a->sign;
    char *result = calloc(len, sizeof(char));
    if (result == NULL)
        return NULL;

    if (a->sign == MINUS)
        result[0] = '-';

    for (int i = 0; i < a->length; i++)
        result[len - i - 1] = '0' + a->digits[i];

    return result;
}

