#!/bin/sh
set -eu

cd "$(dirname "$0")"
export LC_ALL=C
compiler=${CC:-clang}
mkdir -p build

build_variant() {
    variant=$1
    shift
    "$compiler" -std=c11 -Wall -Wextra "$@" -Rpass=loop-vectorize \
        -S wave_update.c -o "build/$variant.s"
    "$compiler" -std=c11 -Wall -Wextra "$@" -c wave_update.c \
        -o "build/$variant.o"
    "$compiler" build/bench.o "build/$variant.o" -o "build/$variant"
}

"$compiler" -std=c11 -Wall -Wextra -O2 -c bench.c -o build/bench.o
for level in 0 1 2 3; do
    build_variant "O$level" "-O$level"
done
build_variant O3-no-vector -O3 -fno-vectorize -fno-slp-vectorize

printf 'run\tbuild\tmicroseconds_per_call\n' > build/timings.tsv
for run in 1 2 3 4 5; do
    for variant in O0 O1 O2 O3 O3-no-vector; do
        timing=$("build/$variant")
        printf '%s\t%s\t%s\n' "$run" "$variant" "$timing" >> build/timings.tsv
    done
done

printf '\n%-18s %s\n' 'Build' 'Median (µs per call)'
for variant in O0 O1 O2 O3 O3-no-vector; do
    median=$(awk -v build="$variant" '$2 == build {print $3}' build/timings.tsv |
        sort -n | sed -n '3p')
    printf '%-18s %s\n' "$variant" "$median"
done
printf '\nRaw timings and assembly are in build/.\n'
