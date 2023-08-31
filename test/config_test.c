#include "log.h"

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

#include "opam.h"
#include "config_test.h"

bool verbose;
int  verbosity;

int main(int argc, char *argv[])
{
    int opt;

    UT_string *opam_file;
    utstring_new(opam_file);

    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
        /* case 'f': */
        /*     /\* BUILD.bazel or BUILD file *\/ */
        /*     utstring_printf(opam_file, "%s", optarg); */
        /*     break; */
        case 'h':
            log_info("Help: ");
            exit(EXIT_SUCCESS);
        /* default: */
            /* log_error("Usage: %s [-f] [opamfile]", argv[0]); */
            /* exit(EXIT_FAILURE); */
        }
    }

    char *wd = getenv("BUILD_WORKING_DIRECTORY");
    if (wd) {
        /* we launched from bazel workspace, cd to launch dir */
        chdir(wd);
    }

    char *switch_name = opam_switch_name();
    log_debug("switch name: %s", switch_name);

    char *site_lib = opam_switch_site_lib();
    log_debug("site-lib: %s", site_lib);

    char *ocaml_version = opam_switch_ocaml_version();
    log_debug("ocaml_version: %s", ocaml_version);

    char *compiler_version = opam_switch_compiler_version();
    log_debug("compiler_version: %s", compiler_version);

    log_debug("config_test exiting");
}