#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
output=$(mktemp -d)
trap 'rm -rf "$output"' EXIT
base=Packages/OpenVanilla/Sources
clang++ -std=c++11 -I"$base/OpenVanilla/include" -I"$base/OVIMTableBased/include" -I"$base/OpenVanillaImpl/include" \
    Tests/table-based-width.cpp "$base/OVIMTableBased/OVIMTableBased.cpp" \
    "$base/OVIMTableBased/OVIMTableBasedContext.cpp" \
    "$base/OpenVanillaImpl/OVConcreteKeyImpl.cpp" "$base/OpenVanillaImpl/OVTextBufferImpl.cpp" \
    -o "$output/test"
"$output/test"
