mkdir -p data
cd data
wget https://www.cs.ubc.ca/~hoos/SATLIB/Benchmarks/SAT/DIMACS/JNH/jnh.tar.gz
for f in *.tar.gz; do tar -xzvf "$f"; done
rm -r *.tar.gz
cd ..
