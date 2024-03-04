#ifndef _CUT_H
#define _CUT_H

#include <stdio.h>

int _cut_ok = 0;
int _cut_failed = 0;

#define _cut_printf(...) printf(__VA_ARGS__)
#define _cut_success(...) _cut_printf("\x1B[32m" __VA_ARGS__ "\x1B[0m")
#define _cut_fail(...) _cut_printf("\x1B[31m" __VA_ARGS__ "\x1B[0m")

#define check(condition) \
  do { \
    if (!(condition)) { \
      _cut_printf(#condition " (line: %d) ", __LINE__) ; \
      return -1; \
    } \
  } while(0)

#define TEST(module, name) int _cut_test_ ## module ## _ ## name()
#define test_run(module, name) \
  do { \
    _cut_printf("\t- Running test " #name ": "); \
    if (_cut_test_ ## module ## _ ## name()) { \
      _cut_failed++; \
      _cut_fail("failed\n"); \
    } else { \
      _cut_ok++; \
      _cut_success("ok\n"); \
    } \
  } while(0)

#define TESTS(module) void _cut_tests_ ## module()
#define tests_run(module) \
  do { \
    _cut_printf("Running test module " #module "\n"); \
    _cut_tests_ ## module(); \
  } while(0)

#define tests_info() \
  do { \
    _cut_printf("Executed %d tests: %d success, %d failed\n", _cut_ok + _cut_failed, _cut_ok, _cut_failed); \
    exit(_cut_failed == 0); \
  } while(0)

#endif

