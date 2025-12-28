#!/usr/bin/env bash
set -e

DEBUG_BIN=./bin/debug
C_FLAGS="-g -Iinclude"
PROJECT_SRC="main.c $(find src -name '*.c')"

mkdir -p bin

case "$1" in
    debug)
        gcc $C_FLAGS $PROJECT_SRC -o $DEBUG_BIN
        ;;
    test_create_response_line)
        gcc $C_FLAGS ./tests/test_request_line_parser.c ./src/parsers/request_line_parser.c -o $DEBUG_BIN && $DEBUG_BIN
        ;;
esac