#include "bigint.h"

#include <string.h>
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
}

TESTS(bigint) {
    test_run(bigint, to_from_string);
}

int main() {
    tests_run(bigint);
}

