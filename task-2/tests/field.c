#include <cut.h>
#include <field.h>
#include <stdlib.h>

const uint8_t f2_p[] = {1};
const uint8_t f2p2_p[] = {1, 1, 1};
const uint8_t f2p3_p[] = {1, 1, 0, 1};
const uint8_t f3_p[] = {1, 1};
const uint8_t f5_p[] = {1, 1};
const uint8_t f7_p[] = {1, 1};

#define create_f2() create_fq(2, 1, f2_p)
#define create_f2p2() create_fq(2, 2, f2p2_p)
#define create_f2p3() create_fq(2, 3, f2p3_p)
#define create_f3() create_fq(3, 1, f3_p)
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

TESTS(field) {
    test_run(field, create_destroy);
    test_run(field, get_zero);
    test_run(field, get_identity);
}

int main() {
    tests_run(field);
    tests_info();
    return 0;
}
