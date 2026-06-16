#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
TESTING_DIR="$REPO_ROOT/testing"

echo "=== Generating testing directory trees ==="

rm -rf "$TESTING_DIR/wide_shallow" \
       "$TESTING_DIR/deep_narrow" \
       "$TESTING_DIR/mixed_project" \
       "$TESTING_DIR/no_target"

mkdir -p "$TESTING_DIR/wide_shallow" \
         "$TESTING_DIR/deep_narrow" \
         "$TESTING_DIR/mixed_project" \
         "$TESTING_DIR/no_target"

echo "--- Creating wide_shallow ---"
WIDE="$TESTING_DIR/wide_shallow"
mkdir -p "$WIDE"
for i in $(seq 1 50); do
    dir_name=$(printf "dir_%03d" "$i")
    mkdir -p "$WIDE/$dir_name"
    for j in $(seq 1 3); do
        printf "dummy content %03d %d\n" "$i" "$j" > "$WIDE/$dir_name/file_${j}.txt"
    done
done
printf "this is the target file in wide_shallow\n" > "$WIDE/dir_050/target.txt"
printf "wide_shallow: %d dirs, target at dir_050\n" 50

echo "--- Creating deep_narrow ---"
DEEP="$TESTING_DIR/deep_narrow"
current="$DEEP"
for i in $(seq 1 30); do
    dir_name=$(printf "level_%03d" "$i")
    current="$current/$dir_name"
    mkdir -p "$current"
    if [ "$i" -eq 30 ]; then
        printf "this is the target file at depth 30\n" > "$current/target.txt"
    fi
done

for i in $(seq 1 30); do
    depth_path="$DEEP"
    for j in $(seq 1 "$i"); do
        dir_name=$(printf "level_%03d" "$j")
        depth_path="$depth_path/$dir_name"
    done
    printf "dummy at depth %03d\n" "$i" > "$depth_path/dummy_${i}.txt"
done
printf "deep_narrow: 30 levels deep with target at bottom\n"

echo "--- Creating mixed_project ---"
MIXED="$TESTING_DIR/mixed_project"
mkdir -p "$MIXED"/{src/{core,utils,traversal/deep},include,docs,tests,assets,build}
printf "README for mixed project\n" > "$MIXED/README.md"
printf "/* main entry */\nint main(void) { return 0; }\n" > "$MIXED/src/main.c"
printf "/* core logic */\nvoid core_init(void) { }\n" "$MIXED/src/core/core.c" > "$MIXED/src/core/core.c" 2>/dev/null || printf "/* core logic */\nvoid core_init(void) { }\n" > "$MIXED/src/core/core.c"
printf "/* core header */\nvoid core_init(void);\n" > "$MIXED/src/core/core.h"
printf "/* utility functions */\nint util_add(int a, int b) { return a + b; }\n" > "$MIXED/src/utils/utils.c"
printf "/* utils header */\nint util_add(int a, int b);\n" > "$MIXED/src/utils/utils.h"
printf "/* traversal */\n" > "$MIXED/src/traversal/traverse.c"
printf "/* traversal header */\n" > "$MIXED/src/traversal/traverse.h"
printf "this is the target file in mixed_project\n" > "$MIXED/src/traversal/deep/target.txt"
printf "/* deep traversal impl */\n" > "$MIXED/src/traversal/deep/deep_traverse.c"
printf "/* deep header */\n" > "$MIXED/src/traversal/deep/deep_traverse.h"
printf "# API definitions\n" > "$MIXED/include/api.h"
printf "# common types\n" > "$MIXED/include/types.h"
printf "# Documentation\n\n## Overview\nThis is a sample project.\n" > "$MIXED/docs/design.md"
printf "## API Reference\n\nFunctions go here.\n" > "$MIXED/docs/api.md"
printf "# test suite\n" > "$MIXED/tests/test_main.c"
printf "# test utils\n" > "$MIXED/tests/test_utils.c"
printf "build artifact\n" > "$MIXED/build/output.log"
printf "asset data 1\n" > "$MIXED/assets/data.txt"
printf "asset data 2\n" > "$MIXED/assets/config.txt"
printf "mixed_project: realistic project structure, target at src/traversal/deep/\n"

echo "--- Creating no_target ---"
NOTARG="$TESTING_DIR/no_target"
mkdir -p "$NOTARG"/{a,b,c}/{1,2,3}/{x,y,z}
for dir in $(find "$NOTARG" -type d); do
    for k in 1 2; do
        printf "dummy %s\n" "$k" > "$dir/file_${k}.txt"
    done
done
printf "no_target: %d files, no target.txt present\n" "$(find "$NOTARG" -type f | wc -l)"

echo ""
echo "=== Testing trees generated successfully ==="
echo "wide_shallow:  $(find "$WIDE" -type d | wc -l) dirs, $(find "$WIDE" -type f | wc -l) files"
echo "deep_narrow:   $(find "$DEEP" -type d | wc -l) dirs, $(find "$DEEP" -type f | wc -l) files"
echo "mixed_project: $(find "$MIXED" -type d | wc -l) dirs, $(find "$MIXED" -type f | wc -l) files"
echo "no_target:     $(find "$NOTARG" -type d | wc -l) dirs, $(find "$NOTARG" -type f | wc -l) files"
