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
TEST(bigint, div) {
    dec_t a = dec_from_string("17");
    dec_t b = dec_from_string("-6");
    dec_t c = dec_from_string("5");
    dec_t d = dec_from_string("24");
    dec_t e = dec_from_string("-29");
    dec_t f;
    dec_t g;
    dec_t h;
    dec_t i;
    dec_t f1;
    dec_t g1;

    dec_t j = dec_div(a, b, &f);
    dec_t k = dec_div(b, c, &g);
    dec_t l = dec_div(e, b, &h);
    dec_t m = dec_div(d, c, &i);

    dec_t j1 = dec_div(a, a, &f1);
    dec_t k1 = dec_div(b, b, &g1);

    char *fs = dec_to_string(f);
    char *gs = dec_to_string(g);
    char *hs = dec_to_string(h);
    char *is = dec_to_string(i);
    char *js = dec_to_string(j);
    char *ks = dec_to_string(k);
    char *ls = dec_to_string(l);
    char *ms = dec_to_string(m);

    char *f1s = dec_to_string(f1);
    char *g1s = dec_to_string(g1);

    char *j1s = dec_to_string(j1);
    char *k1s = dec_to_string(k1);

    check(!strcmp(js, "-2"));
    check(!strcmp(fs, "5"));

    check(!strcmp(ks, "-2"));
    check(!strcmp(gs, "4"));

    check(!strcmp(ls, "5"));
    check(!strcmp(hs, "1"));

    check(!strcmp(ms, "4"));
    check(!strcmp(is, "4"));

    check(!strcmp(j1s, "1"));
    check(!strcmp(f1s, "0"));

    check(!strcmp(k1s, "1"));
    check(!strcmp(g1s, "0"));

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    destroy_dec(e);
    destroy_dec(f);
    destroy_dec(g);
    destroy_dec(h);
    destroy_dec(i);
    destroy_dec(j);
    destroy_dec(k);
    destroy_dec(l);
    destroy_dec(m);

    destroy_dec(f1);
    destroy_dec(g1);

    destroy_dec(j1);
    destroy_dec(k1);

    free(fs);
    free(gs);
    free(hs);
    free(is);
    free(js);
    free(ks);
    free(ls);
    free(ms);

    free(f1s);
    free(g1s);

    free(j1s);
    free(k1s);

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
dec_t fac(size_t n) {
    if (n == 0)
        return dec_from_int(1);
    dec_t a = fac(n - 1);
    dec_t b = dec_from_int(n);
    dec_t result = dec_mul(a, b);
    destroy_dec(a);
    destroy_dec(b);
    return result;
}
dec_t euc(cdec_t a, cdec_t b) {
    dec_t zero = dec_from_int(0);
    if (dec_cmp(a, zero) == 0) {
        destroy_dec(zero);
        return dec_copy(b);
    } else if (dec_cmp(b, zero) == 0) {
        destroy_dec(zero);
        return dec_copy(a);
    }
    destroy_dec(zero);

    int8_t cmp = dec_cmp(a, b);
    if (cmp == 1) {
        dec_t r;
        dec_t c = dec_div(a, b, &r);
        destroy_dec(c);
        dec_t result = euc(r, b);
        destroy_dec(r);
        return result;
    } else if (cmp == -1) {
        dec_t r;
        dec_t c = dec_div(b, a, &r);
        destroy_dec(c);
        dec_t result = euc(r, a);
        destroy_dec(r);
        return result;
    } else {
        return dec_copy(a);
    }
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
TEST(bigint, fac) {
    dec_t a = fac(1);
    dec_t b = fac(2);
    dec_t c = fac(10);
    dec_t d = fac(100);

    char *as = dec_to_string(a);
    char *bs = dec_to_string(b);
    char *cs = dec_to_string(c);
    char *ds = dec_to_string(d);

    check(!strcmp(as, "1"));
    check(!strcmp(bs, "2"));
    check(!strcmp(cs, "3628800"));
    check(!strcmp(ds, "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000"));

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
TEST(bigint, euc) {
    dec_t a = dec_from_string("123456789123456789123456789");
    dec_t b = dec_from_string("7");
    dec_t c = dec_from_string("333333333333333333333333333");
    dec_t d = dec_from_string("3055226168111655637560126414086140721666308321352217707326012376644322994342882916399289316897113845");
    dec_t e = dec_from_string("7036420146916556032731470518270036236016957588191783967261853431201878204048778528737031671137311690");

    dec_t g = euc(a, c);
    dec_t h = euc(c, b);
    dec_t i = euc(d, e);

    char *gs = dec_to_string(g);
    char *hs = dec_to_string(h);
    char *is = dec_to_string(i);

    check(!strcmp(gs, "9000000009000000009"));
    check(!strcmp(hs, "1"));
    check(!strcmp(is, "5"));

    destroy_dec(a);
    destroy_dec(b);
    destroy_dec(c);
    destroy_dec(d);
    destroy_dec(e);
    destroy_dec(g);
    destroy_dec(h);
    destroy_dec(i);
    free(gs);
    free(hs);
    free(is);

    return 0;
}

TESTS(bigint) {
    test_run(bigint, to_from_string);
    test_run(bigint, add);
    test_run(bigint, sub);
    test_run(bigint, mul);
    test_run(bigint, div);
    test_run(bigint, cmp);
    test_run(bigint, fib);
    test_run(bigint, fac);
    test_run(bigint, euc);
}

int main() {
    tests_run(bigint);
}

