#!/bin/sh

# usage: check-includes.sh DIR...
# <...> includes must be <kernel/...>, <driver/...> or <asm/...>

bad=$(grep -rnsE --include='*.[chS]' \
    '^[[:space:]]*#[[:space:]]*include[[:space:]]*<' "$@" |
    grep -vE '<(kernel|driver|asm)/')

if [ -n "$bad" ]; then
    echo "$bad"
    echo "  FAIL  includes must be <kernel/...>, <driver/...> or <asm/...>"
    exit 1
fi

echo "  OK    include paths"
