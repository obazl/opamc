#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>

#if INTERFACE
#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#endif

#include <unistd.h>

#include "log.h"

#include "utarray.h"
#include "utstring.h"
#include "utstring.h"

#include "findlib.h"
#include "bazel_test.h"

#include "log.h"

bool verbose;
int  verbosity;

UT_string *meta_path;

int main(int argc, char *argv[])
{
    int opt;

    utstring_new(meta_path);
    char *opam_switch = NULL;

    /* char *homedir = getenv("HOME"); */

    while ((opt = getopt(argc, argv, "s:hv")) != -1) {
        switch (opt) {
        case 's':
            /* BUILD.bazel or BUILD file */
            opam_switch = optarg;
            break;
        case 'h':
            log_info("Help: ");
            exit(EXIT_SUCCESS);
        /* default: */
        /*     log_error("Usage: %s -s <opam switch name>", argv[0]); */
        /*     exit(EXIT_FAILURE); */
        }
    }

    /* if (opam_switch == NULL) { */
    /*     log_error("-s <opam switch name> must be provided."); */
    /*     exit(EXIT_FAILURE); */
    /* } else { */
    /*     log_info("opam switch: %s", opam_switch); */
    /* } */

    char *wd = getenv("BUILD_WORKING_DIRECTORY");
    if (wd) {
        /* we launched from bazel workspace, cd to launch dir */
        chdir(wd);
    }

    bazel_map(opam_switch);

    /* UT_array *opam_include_pkgs; */
    /* utarray_new(opam_include_pkgs,&ut_str_icd); */

    /* UT_array *opam_exclude_pkgs; */
    /* utarray_new(opam_exclude_pkgs,&ut_str_icd); */

    /* crawl_findlib_repo(opam_include_pkgs, */
    /*                    opam_exclude_pkgs, */
    /*                    utstring_body(opam_switch), */
    /*                    findlib_pkg_handler); */
    /* utstring_free(opam_switch); */
    /* utstring_free(meta_path); */
    log_info("bazel_test exiting");
}
