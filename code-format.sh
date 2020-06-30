set -e

which clang-format 2>&1 >/dev/null || \
    ( echo "clang-format is not installed, please install it and try again" >&2; exit 1 )

find . -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp \
    | xargs clang-format -style=file -i -fallback-style=none

