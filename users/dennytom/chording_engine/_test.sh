#!/usr/bin/bash

# indent test.c -bad -bap -bbb -br -brf -brs -ce -i4 -l100 -nut -sob

python3 expander3.py -f test.c.in | cat -s > test.c && \
gcc -g test.c -o test && \
./test.exe && \
cd ../../../.. && \
./_tomas.sh && \
make butterstick:default && \
cd keyboards/georgi/keymaps/buttery && \
python3 expander3.py -f keymap.c.in | cat -s > keymap.c && \
cd ../../../.. && \
make georgi:buttery && \
cd c:/dev/qmk/keyboards/butterstick/keymaps/tomas