#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <libgen.h>
#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>             /* PATH_MAX */
#endif
#include <pwd.h>
#include <spawn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

/* #include "ini.h" */
#include "liblogc.h"
/* #if EXPORT_INTERFACE */
#include "utarray.h"
#include "utstring.h"
/* #endif */

#include "opam_config.h"

/* const char *errmsg = NULL; */

const char *opamc_version = OPAMC_VERSION;

extern bool verbose;
extern int  verbosity;

#if defined(PROFILE_fastbuild)
#define DEBUG_LEVEL opamc_debug
int  DEBUG_LEVEL;
#define TRACE_FLAG opamc_trace
bool TRACE_FLAG;
#endif

extern int rc;

/* char *bazel_script_dir = NULL; */

extern bool enable_jsoo;

/* ******************************** */
EXPORT char *opam_root(void)
{
    TRACE_ENTRY
    char *result = NULL;
    char *exe = "opam";
    char *argv[] = {"opam", "var", "root", NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        fprintf(stderr, "FAIL: run_cmd 'opam var root'\n");
    }
    return result;
}

/* ******************************** */
EXPORT char *opam_switch_name(void)
{
    TRACE_ENTRY
    char *result = NULL;

    char *exe = "opam";
    // or" 'opam var switch'
    char *argv[] = {"opam", "switch", "show", NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        log_error("FAIL: run_cmd 'opam var switch'");
        fprintf(stderr, "FAIL: run_cmd 'opam var switch'\n");
    /* } else { */
    /*     log_debug("opam var switch: %s", result); */
    }
    return result;
}

EXPORT char *opam_switch_prefix(char *opam_switch)
{
    TRACE_ENTRY;
    char *result = NULL;
    char *exe = "opam";
    char *argv[] = {"opam", "var", "prefix", "--switch", opam_switch, NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        fprintf(stderr, "FAIL: run_cmd 'opam var prefix --switch'\n");
    }
    return result;
}

EXPORT char *opam_switch_lib(char *opam_switch)
{
    TRACE_ENTRY;
    char *result = NULL;
    char *exe = "opam";
    char *argv[] = {"opam", "var", "lib", "--switch", opam_switch, NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        fprintf(stderr, "FAIL: run_cmd 'opam var lib --switch'\n");
    }
    TRACE_EXIT;
    return result;
}

EXPORT char *opam_switch_stublibs(char *opam_switch)
{
    TRACE_ENTRY;
    char *result = NULL;
    char *exe = "opam";
    char *argv[] = {"opam", "var", "stublibs", "--switch", opam_switch, NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        fprintf(stderr, "FAIL: run_cmd 'opam var stublibs --switch'\n");
    }
    return result;
}

EXPORT char *opam_switch_ocaml_version(char *opam_switch)
{
    TRACE_ENTRY
    char *result = NULL;
    char *exe = "opam";
    char *argv[] = {"opam", "var", "ocaml:version", "--switch", opam_switch, NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        fprintf(stderr, "FAIL: run_cmd 'opam var ocaml:version'\n");
    }
    return result;
}

EXPORT char *opam_switch_base_compiler_version(char *opam_switch)
{
    TRACE_ENTRY
    char *result = NULL;
    char *exe = "opam";
    char *argv[] = {"opam", "var", "ocaml-base-compiler:version", "--switch", opam_switch, NULL};
    result = run_cmd(exe, argv);
    if (result == NULL) {
        fprintf(stderr,
                RED "ERROR" CRESET
                " %s:%d run_cmd 'opam var ocaml-base-compiler:version --switch %s'\n",
                __FILE__, __LINE__, opam_switch);
    }
    return result;
}

