#include "bigint.h"

#include <string.h>
#include <stdlib.h>
#include "cut.h"

TEST(bigint, to_from_string) {
    dec_t a = dec_from_string("1234");
    dec_t b = dec_from_string("987654321987654321987654321");
    dec_t c = dec_from_string("-987654321987654321987654321");
    dec_t d = dec_from_string("-1234");
    dec_t e = dec_from_string("0");
    dec_t f = dec_from_string("-0");
    dec_t g = dec_from_string(".0");
    dec_t h = dec_from_string("1234e44");

    char *as = dec_to_string(a);
    char *bs = dec_to_string(b);
    char *cs = dec_to_string(c);
    char *ds = dec_to_string(d);
    char *es = dec_to_string(e);
    char *fs = dec_to_string(f);

    check(!strcmp(as, "1234"));
    check(!strcmp(bs, "987654321987654321987654321"));
    check(!strcmp(cs, "-987654321987654321987654321"));
    check(!strcmp(ds, "-1234"));
    check(!strcmp(es, "0"));
    check(!strcmp(fs, "0"));
    check(g == NULL);
    check(h == NULL);

    destroy_dec(a);
    free(as);
    destroy_dec(b);
    free(bs);
    destroy_dec(c);
    free(cs);
    destroy_dec(d);
    free(ds);
    destroy_dec(e);
    free(es);
    destroy_dec(f);
    free(fs);

    return 0;
}
TEST(bigint, add) {
    dec_t a = dec_from_string("123456789123456789123456789");
    dec_t b = dec_from_string("-123456789123456789123456789");
    dec_t c = dec_from_string("333333333333333333333333333");
    dec_t d = dec_from_string("-333333333333333333333333333");
    dec_t e = dec_from_string("0");
    dec_t f = dec_add(a, c);
    dec_t g = dec_add(a, b);
    dec_t h = dec_add(d, b);
    dec_t i = dec_add(a, e);

    char *fs = dec_to_string(f);
    char *gs = dec_to_string(g);
    char *hs = dec_to_string(h);
    char *is = dec_to_string(i);

    check(!strcmp(fs, "456790122456790122456790122"));
    check(!strcmp(gs, "0"));
    check(!strcmp(hs, "-456790122456790122456790122"));
    check(!strcmp(is, "123456789123456789123456789"));

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    destroy_dec(e);
    destroy_dec(f);
    destroy_dec(g);
    destroy_dec(h);
    destroy_dec(i);
    free(fs);
    free(gs);
    free(hs);
    free(is);

    return 0;
}

TEST(bigint, sub) {
    dec_t a = dec_from_string("123456789123456789123456789");
    dec_t b = dec_from_string("-123456789123456789123456789");
    dec_t c = dec_from_string("333333333333333333333333333");
    dec_t d = dec_from_string("-333333333333333333333333333");
    dec_t e = dec_from_string("123456789123456789123456789");
    dec_t f = dec_sub(a, d);
    dec_t g = dec_sub(b, c);
    dec_t h = dec_sub(a, e);

    char *fs = dec_to_string(f);
    char *gs = dec_to_string(g);
    char *hs = dec_to_string(h);

    check(!strcmp(fs, "456790122456790122456790122"));
    check(!strcmp(gs, "-456790122456790122456790122"));
    check(!strcmp(hs, "0"));

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    destroy_dec(e);
    destroy_dec(f);
    destroy_dec(g);
    destroy_dec(h);
    free(fs);
    free(gs);
    free(hs);

    return 0;
}
TEST(bigint, mul) {
    dec_t a = dec_from_string("123456789123456789123456789");
    dec_t b = dec_from_string("-123456789123456789123456789");
    dec_t c = dec_from_string("333333333333333333333333333");
    dec_t d = dec_from_string("-333333333333333333333333333");
    dec_t e = dec_from_string("0");
    dec_t f = dec_mul(a, c);
    dec_t g = dec_mul(a, d);
    dec_t h = dec_mul(a, e);
    dec_t i = dec_mul(b, d);

    char *fs = dec_to_string(f);
    char *gs = dec_to_string(g);
    char *hs = dec_to_string(h);
    char *is = dec_to_string(i);

    check(!strcmp(fs, "41152263041152263041152262958847736958847736958847737"));
    check(!strcmp(gs, "-41152263041152263041152262958847736958847736958847737"));
    check(!strcmp(hs, "0"));
    check(!strcmp(is, "41152263041152263041152262958847736958847736958847737"));

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    destroy_dec(e);
    destroy_dec(f);
    destroy_dec(g);
    destroy_dec(h);
    destroy_dec(i);
    free(fs);
    free(gs);
    free(hs);
    free(is);

    return 0;
}

TEST(bigint, cmp) {
    dec_t a = dec_from_string("123456789123456789123456789");
    dec_t b = dec_from_string("-123456789123456789123456789");
    dec_t c = dec_from_string("333333333333333333333333333");
    dec_t d = dec_from_string("-333333333333333333333333333");
    dec_t e = dec_from_string("0");
    dec_t f = dec_from_string("-0");

    check(dec_cmp(a, a) == 0);
    check(dec_cmp(a, b) == 1);
    check(dec_cmp(b, a) == -1);
    check(dec_cmp(e, f) == 0);
    check(dec_cmp(f, e) == 0);
    check(dec_cmp(a, c) == -1);
    check(dec_cmp(c, a) == 1);
    check(dec_cmp(b, d) == 1);
    check(dec_cmp(d, b) == -1);

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    destroy_dec(e);
    destroy_dec(f);

    return 0;
}

dec_t fib(size_t n) {
    dec_t *temp = calloc(n + 3, sizeof(dec_t));
    temp[0] = dec_from_int(0);
    temp[1] = dec_from_int(1);
    for (size_t i = 2; i <= n; i++)
        temp[i] = dec_add(temp[i - 1], temp[i - 2]);
    for (size_t i = 0; i < n; i++)
        destroy_dec(temp[i]);
    dec_t result = temp[n];
    free(temp);
    return result;
}
TEST(bigint, fib) {
    dec_t a = fib(1);
    dec_t b = fib(2);
    dec_t c = fib(10);
    dec_t d = fib(100);

    char *as = dec_to_string(a);
    char *bs = dec_to_string(b);
    char *cs = dec_to_string(c);
    char *ds = dec_to_string(d);

    check(!strcmp(as, "1"));
    check(!strcmp(bs, "1"));
    check(!strcmp(cs, "55"));
    check(!strcmp(ds, "354224848179261915075"));

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    free(as);
    free(bs);
    free(cs);
    free(ds);

    return 0;
}

TESTS(bigint) {
    test_run(bigint, to_from_string);
    test_run(bigint, add);
    test_run(bigint, sub);
    test_run(bigint, mul);
    test_run(bigint, cmp);
    test_run(bigint, fib);
}

int main() {
    tests_run(bigint);
}

