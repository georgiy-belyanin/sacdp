Finite field library
====================

A simple C library that can be used to work with finite fields.

Building:
--------
```bash
mkdir build
cd build
cmake ..
make
```

Testing:
--------
```bash
mkdir build
cd build
cmake ..
make
make test
```

Example usage:
--------------
```c
int main() {
    // Define some irreducible polynomial over F3
    uint8_t f3p3_p[] = {1, 2, 0, 1};
    // Create a new field, based on F3[x]/(x^3 + 2x + 1)
    fq_t another_field = create_fq(3, 3, f3p3_p);
    // Get an element of F2P8 with coeffecients, equal to binary representation of 17
    f_t a = f_from_uint8(17);
    // Get an inverse element to the first
    f_t b = f_inv(a);
    // Multiyply a element by b element
    f_t c = f_mul(b, c);

    // Print a finite field element to stdout
    f_print(b);
    // Expecting one here
    f_print(a);

    // Do some cleanup...
    destroy_f(a);
    destroy_f(b);
    destroy_f(c);

    destroy_fq(another_field);

    return 0;
}
```

