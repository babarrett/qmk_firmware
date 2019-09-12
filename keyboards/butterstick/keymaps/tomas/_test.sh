#!/usr/bin/bash
python3 expander3.py -f test.c.in | cat -s > test.c && \
indent test.c -bad -bap -bbb -br -brf -brs -ce -i4 -l100 -nut -sob && \
gcc -g test.c -o test && \
./test.exe && \
cd ../../../.. && \
make butterstick && \
make georgi:buttery

cd c:/dev/qmk/keyboards/butterstick/keymaps/tomas