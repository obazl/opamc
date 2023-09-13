load("@bazel_skylib//lib:paths.bzl", "paths")

# CMD_FLAGS = [
#     "-std=c11",
#     "-pedantic-errors",
# ] + select({
#     "//bzl/host:linux": [
#         "-D_POSIX_C_SOURCE=200809L", ## strdup, strndup
#         "-D_DEFAULT_SOURCE"],        ## DT_ constants from dirent.h
#     "//conditions:default":   []
# })

## macos: -UDEBUG

# BOOTSTRAP_INCLUDES = []
#     "-I.",
#     "-Ibootstrap",
#     "-Iexternal/opam/bootstrap",

#     "-I$(GENDIR)",
#     "-I$(GENDIR)/bootstrap",
#     "-I$(GENDIR)/opam/bootstrap",
#     "-I$(GENDIR)/external/opam/bootstrap",
# ]

################################################################
debug_defs = select({
    "//:debug-ast": ["DEBUG_AST"],
    "//conditions:default":   []
}) + select({
    "//:debug-bindings": ["DEBUG_BINDINGS"],
    "//conditions:default":   []
}) + select({
    "//:debug-ctors": ["DEBUG_CTORS"],
    "//conditions:default":   []
}) + select({
    "//:debug-filters": ["DEBUG_FILTERS"],
    "//conditions:default":   []
}) + select({
    "//:debug-format": ["DEBUG_FORMAT"],
    "//conditions:default":   []
}) + select({
    "//:debug-load": ["DEBUG_LOAD"],
    "//conditions:default":   []
}) + select({
    "//:debug-loads": ["DEBUG_LOADS"],
    "//conditions:default":   []
}) + select({
    "//:debug-mem": ["DEBUG_MEM"],
    "//conditions:default":   []
}) + select({
    "//:debug-mutators": ["DEBUG_MUTATORS"],
    "//conditions:default":   []
}) + select({
    "//:debug-paths": ["DEBUG_PATHS"],
    "//conditions:default":   []
}) + select({
    "//:debug-preds": ["DEBUG_PREDICATES"],
    "//conditions:default":   []
}) + select({
    "//:debug-properties": ["DEBUG_PROPERTIES"],
    "//conditions:default":   []
}) + select({
    "//:debug-queries": ["DEBUG_QUERY"],
    "//conditions:default":   []
}) + select({
    "//:debug-s7-api": ["DEBUG_S7_API"],
    "//conditions:default":   []
}) + select({
    "//:debug-set": ["DEBUG_SET"],
    "//conditions:default":   []
}) + select({
    "//:debug-serializers": ["DEBUG_SERIALIZERS"],
    "//conditions:default":   []
}) + select({
    "//:debug-targets": ["DEBUG_TARGETS"],
    "//conditions:default":   []
}) + select({
    "//:debug-trace": ["TRACING"],
    "//conditions:default":   []
}) + select({
    "//:debug-utarrays": ["DEBUG_UTARRAYS"],
    "//conditions:default":   []
}) + select({
    "//:debug-vectors": ["DEBUG_VECTORS"],
    "//conditions:default":   []
}) + select({
    "//:debug-yytrace": ["DEBUG_YYTRACE"],
    "//conditions:default":   []
})

