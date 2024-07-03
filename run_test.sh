#!/bin/sh
set -e
gcc -lcurl -lz tests/test.c -o tests/test
chmod +x tests/test
exec tests/test
