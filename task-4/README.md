C DPLL SAT Solver
====================

A simple implementation of DPLL algorithm.

Building:
--------
```bash
mkdir build
cd build
cmake ..
make
```

Usage:
--------------
```bash
./dpll <DIMACS .cnf file>
```

Download test data:
-------------------
```bash
./download.sh
```
It downloads JNF DIMACS file set.

Comparing with picosat:
--------
Compare our impelmentation with picosat (requires python3 and downloaded test data):
```bash
python3 compare.py
```


