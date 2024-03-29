#include <cut.h>
#include <field.h>
#include <stdlib.h>

const uint8_t f2_p[] = {1, 1};
const uint8_t f2p2_p[] = {1, 1, 1};
const uint8_t f2p3_p[] = {1, 1, 0, 1};
const uint8_t f3_p[] = {1, 1};
const uint8_t f3p3_p[] = {1, 2, 0, 1};
const uint8_t f5_p[] = {1, 1};
const uint8_t f7_p[] = {1, 1};

#define create_f2() create_fq(2, 1, f2_p)
#define create_f2p2() create_fq(2, 2, f2p2_p)
#define create_f2p3() create_fq(2, 3, f2p3_p)
#define create_f3() create_fq(3, 1, f3_p)
#define create_f3p3() create_fq(3, 3, f3p3_p)
#define create_f5() create_fq(5, 1, f5_p)
#define create_f7() create_fq(7, 1, f5_p)

TEST(field, create_destroy) {
    fq_t a = create_f2();
    fq_t b = create_f2p2();
    fq_t c = create_f2p3();
    fq_t d = create_f3();
    fq_t e = create_f5();

    check(a != NULL);
    check(b != NULL);
    check(c != NULL);
    check(d != NULL);
    check(e != NULL);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);
    destroy_fq(d);
    destroy_fq(e);

    return 0;
}

TEST(field, get_zero) {
    fq_t a = create_f2();
    fq_t b = create_f2p2();

    f_t c = fq_get_zero(a);
    f_t d = fq_get_zero(b);
    uint8_t ep[] = {0};
    f_t e = fq_get_from_array(a, ep);
    uint8_t fp[] = {0, 0};
    f_t f = fq_get_from_array(b, fp);

    check(f_eq(c, e));
    check(f_eq(d, f));

    destroy_f(c);
    destroy_f(d);
    destroy_f(e);
    destroy_f(f);

    destroy_fq(a);
    destroy_fq(b);

    return 0;
}

TEST(field, get_identity) {
    fq_t a = create_f2();
    fq_t b = create_f2p2();

    f_t c = fq_get_identity(a);
    f_t d = fq_get_identity(b);
    uint8_t ep[] = {1};
    f_t e = fq_get_from_array(a, ep);
    uint8_t fp[] = {1, 0};
    f_t f = fq_get_from_array(b, fp);

    check(f_eq(c, e));
    check(f_eq(d, f));

    destroy_f(c);

    destroy_f(d);
    destroy_f(e);
    destroy_f(f);

    destroy_fq(a);
    destroy_fq(b);

    return 0;
}

TEST(field, add) {
    fq_t a = create_f2p3();
    fq_t b = create_f7();
    fq_t c = create_f3p3();

    uint8_t dp[] = {1, 1, 1};
    f_t d = fq_get_from_array(a, dp);
    uint8_t ep[] = {1, 0, 1};
    f_t e = fq_get_from_array(a, ep);
    uint8_t fp[] = {0, 1, 0};
    f_t f = fq_get_from_array(a, fp);
    f_t g = f_add(d, e);

    uint8_t hp[] = {4};
    f_t h = fq_get_from_array(b, hp);
    uint8_t ip[] = {6};
    f_t i = fq_get_from_array(b, ip);
    uint8_t jp[] = {3};
    f_t j = fq_get_from_array(b, jp);
    f_t k = f_add(h, i);

    uint8_t lp[] = {2, 0, 2};
    f_t l = fq_get_from_array(c, lp);
    uint8_t mp[] = {2, 2, 1};
    f_t m = fq_get_from_array(c, mp);
    uint8_t np[] = {1, 2, 0};
    f_t n = fq_get_from_array(c, np);
    f_t o = f_add(l, m);

    check(f_eq(g, f));
    check(f_eq(k, j));
    check(f_eq(n, o));

    destroy_f(d);
    destroy_f(e);
    destroy_f(f);
    destroy_f(g);
    destroy_f(h);
    destroy_f(i);
    destroy_f(j);
    destroy_f(k);
    destroy_f(l);
    destroy_f(m);
    destroy_f(n);
    destroy_f(o);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);

    return 0;
}

TEST(field, sub) {
    fq_t a = create_f2p3();
    fq_t b = create_f7();
    fq_t c = create_f3p3();

    uint8_t dp[] = {1, 1, 1};
    f_t d = fq_get_from_array(a, dp);
    uint8_t ep[] = {1, 0, 1};
    f_t e = fq_get_from_array(a, ep);
    uint8_t fp[] = {0, 1, 0};
    f_t f = fq_get_from_array(a, fp);
    f_t g = f_sub(d, e);

    uint8_t hp[] = {4};
    f_t h = fq_get_from_array(b, hp);
    uint8_t ip[] = {6};
    f_t i = fq_get_from_array(b, ip);
    uint8_t jp[] = {5};
    f_t j = fq_get_from_array(b, jp);
    f_t k = f_sub(h, i);

    uint8_t lp[] = {2, 0, 2};
    f_t l = fq_get_from_array(c, lp);
    uint8_t mp[] = {2, 2, 1};
    f_t m = fq_get_from_array(c, mp);
    uint8_t np[] = {0, 1, 1};
    f_t n = fq_get_from_array(c, np);
    f_t o = f_sub(l, m);

    check(f_eq(g, f));
    check(f_eq(k, j));
    check(f_eq(n, o));

    destroy_f(d);
    destroy_f(e);
    destroy_f(f);
    destroy_f(g);
    destroy_f(h);
    destroy_f(i);
    destroy_f(j);
    destroy_f(k);
    destroy_f(l);
    destroy_f(m);
    destroy_f(n);
    destroy_f(o);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);

    return 0;
}

TEST(field, neg) {
    fq_t a = create_f2p3();
    fq_t b = create_f7();
    fq_t c = create_f3p3();

    uint8_t dp[] = {1, 1, 1};
    f_t d = fq_get_from_array(a, dp);
    uint8_t fp[] = {1, 1, 1};
    f_t f = fq_get_from_array(a, fp);
    f_t g = f_neg(d);

    uint8_t hp[] = {4};
    f_t h = fq_get_from_array(b, hp);
    uint8_t jp[] = {3};
    f_t j = fq_get_from_array(b, jp);
    f_t k = f_neg(h);

    uint8_t lp[] = {2, 0, 2};
    f_t l = fq_get_from_array(c, lp);
    uint8_t np[] = {1, 0, 1};
    f_t n = fq_get_from_array(c, np);
    f_t o = f_neg(l);

    check(f_eq(g, f));
    check(f_eq(k, j));
    check(f_eq(n, o));

    destroy_f(d);
    destroy_f(f);
    destroy_f(g);
    destroy_f(h);
    destroy_f(j);
    destroy_f(k);
    destroy_f(l);
    destroy_f(n);
    destroy_f(o);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);

    return 0;
}

TEST(field, mul) {
    fq_t a = create_f2p3();
    fq_t b = create_f7();
    fq_t c = create_f3p3();

    uint8_t dp[] = {1, 1, 1}; // a^5
    f_t d = fq_get_from_array(a, dp);
    uint8_t ep[] = {1, 0, 1}; // a^6
    f_t e = fq_get_from_array(a, ep);
    uint8_t fp[] = {0, 1, 1}; // a^4
    f_t f = fq_get_from_array(a, fp);
    f_t g = f_mul(d, e);

    uint8_t hp[] = {4};
    f_t h = fq_get_from_array(b, hp);
    uint8_t ip[] = {6};
    f_t i = fq_get_from_array(b, ip);
    uint8_t jp[] = {3};
    f_t j = fq_get_from_array(b, jp);
    f_t k = f_mul(h, i);

    uint8_t lp[] = {2, 0, 2}; // a^8
    f_t l = fq_get_from_array(c, lp);
    uint8_t mp[] = {2, 2, 1}; // a^7
    f_t m = fq_get_from_array(c, mp);
    uint8_t np[] = {0, 0, 2}; // a^15
    f_t n = fq_get_from_array(c, np);
    f_t o = f_mul(l, m);

    check(f_eq(g, f));
    check(f_eq(k, j));
    check(f_eq(n, o));

    destroy_f(d);
    destroy_f(e);
    destroy_f(f);
    destroy_f(g);
    destroy_f(h);
    destroy_f(i);
    destroy_f(j);
    destroy_f(k);
    destroy_f(l);
    destroy_f(m);
    destroy_f(n);
    destroy_f(o);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);

    return 0;
}

TEST(field, div) {
    fq_t a = create_f2p3();
    fq_t b = create_f7();
    fq_t c = create_f3p3();

    uint8_t dp[] = {1, 1, 1}; // a^5
    f_t d = fq_get_from_array(a, dp);
    uint8_t ep[] = {1, 0, 1}; // a^6
    f_t e = fq_get_from_array(a, ep);
    uint8_t fp[] = {1, 0, 1}; // a^6
    f_t f = fq_get_from_array(a, fp);
    f_t g = f_div(d, e);

    uint8_t hp[] = {4};
    f_t h = fq_get_from_array(b, hp);
    uint8_t ip[] = {6};
    f_t i = fq_get_from_array(b, ip);
    uint8_t jp[] = {3};
    f_t j = fq_get_from_array(b, jp);
    f_t k = f_div(h, i);

    uint8_t lp[] = {2, 0, 2}; // a^8
    f_t l = fq_get_from_array(c, lp);
    uint8_t mp[] = {2, 2, 1}; // a^7
    f_t m = fq_get_from_array(c, mp);
    uint8_t np[] = {0, 1, 0}; // a
    f_t n = fq_get_from_array(c, np);
    f_t o = f_div(l, m);

    check(f_eq(g, f));
    check(f_eq(k, j));
    check(f_eq(n, o));

    destroy_f(d);
    destroy_f(e);
    destroy_f(f);
    destroy_f(g);
    destroy_f(h);
    destroy_f(i);
    destroy_f(j);
    destroy_f(k);
    destroy_f(l);
    destroy_f(m);
    destroy_f(n);
    destroy_f(o);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);

    return 0;
}

TEST(field, inv) {
    fq_t a = create_f2p3();
    fq_t b = create_f7();
    fq_t c = create_f3p3();

    uint8_t dp[] = {1, 1, 1}; // a^5
    f_t d = fq_get_from_array(a, dp);
    uint8_t fp[] = {0, 0, 1}; // a^2
    f_t f = fq_get_from_array(a, fp);
    f_t g = f_inv(d);

    uint8_t hp[] = {4};
    f_t h = fq_get_from_array(b, hp);
    uint8_t jp[] = {2};
    f_t j = fq_get_from_array(b, jp);
    f_t k = f_inv(h);

    uint8_t lp[] = {2, 0, 2}; // a^8
    f_t l = fq_get_from_array(c, lp);
    uint8_t np[] = {1, 2, 1}; // a^18
    f_t n = fq_get_from_array(c, np);
    f_t o = f_inv(l);

    check(f_eq(g, f));
    check(f_eq(k, j));
    check(f_eq(n, o));

    destroy_f(d);
    destroy_f(f);
    destroy_f(g);
    destroy_f(h);
    destroy_f(j);
    destroy_f(k);
    destroy_f(l);
    destroy_f(n);
    destroy_f(o);

    destroy_fq(a);
    destroy_fq(b);
    destroy_fq(c);

    return 0;
}

TEST(field, f2p8_16_32) {
    f_t a = f_from_uint8(17);
    f_t b = f_from_uint16(4353);
    f_t c = f_from_uint32(69889);

    uint8_t dp[] = {1, 0, 0, 0, 1, 0, 0, 0};
    f_t d = fq_get_from_array(F2P8, dp);
    uint8_t ep[] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    f_t e = fq_get_from_array(F2P16, ep);
    uint8_t fp[] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    f_t f = fq_get_from_array(F2P32, fp);

    check(f_eq(a, d));
    check(f_eq(b, e));
    check(f_eq(c, f));

    destroy_f(a);
    destroy_f(b);
    destroy_f(c);
    destroy_f(d);
    destroy_f(e);
    destroy_f(f);

    return 0;
}

TESTS(field) {
    test_run(field, create_destroy);
    test_run(field, get_zero);
    test_run(field, get_identity);
    test_run(field, add);
    test_run(field, sub);
    test_run(field, neg);
    test_run(field, mul);
    test_run(field, div);
    test_run(field, inv);
    test_run(field, f2p8_16_32);
}

int main() {
    tests_run(field);
    tests_info();
    return 0;
}

