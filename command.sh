#!/usr/bin/env bash
set -e

DEBUG_BIN=./bin/debug
C_FLAGS="-g -Iinclude"
PROJECT_SRC="main.c $(find src -name '*.c')"

mkdir -p bin

build_debug(){
    gcc $C_FLAGS $PROJECT_SRC -o $DEBUG_BIN
}

case "$1" in
    debug)
        build_debug
        ;;
    dev)
        build_debug && $DEBUG_BIN
        ;;
    test_request_line_parser)
        gcc $C_FLAGS ./tests/parsers/test_request_line_parser.c ./src/parsers/request_line_parser.c -o $DEBUG_BIN && $DEBUG_BIN
        ;;
    test_header_parser)
        gcc $C_FLAGS ./tests/parsers/test_header_parser.c ./src/parsers/header_parser.c -o $DEBUG_BIN && $DEBUG_BIN
        ;;
    test_is_safe_uri)
        gcc $C_FLAGS ./tests/utils/test_is_safe_uri.c ./src/utils/utils.c -o $DEBUG_BIN && $DEBUG_BIN
        ;;
esac