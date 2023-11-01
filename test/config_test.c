#include "liblogc.h"

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

#include "liblogc.h"
#include "utarray.h"
#include "utstring.h"
#include "utstring.h"

#include "opamc.h"
#include "config_test.h"

extern char *opamc_version;

bool verbose;
int  verbosity;

#if defined(PROFILE_fastbuild)
#define DEBUG_LEVEL opamc_debug
extern  int  DEBUG_LEVEL;
#define TRACE_FLAG opamc_trace
extern  bool TRACE_FLAG;
#endif

int main(int argc, char *argv[])
{
    int opt;
    log_debug("libopamc version: %s", opamc_version);
    /* opamc_debug = 3; */
    /* opamc_trace = true; */

    UT_string *opam_file;
    utstring_new(opam_file);

    while ((opt = getopt(argc, argv, "dhv")) != -1) {
        switch (opt) {
#if defined(PROFILE_fastbuild)
        case 'd':
            opamc_debug++;
            break;
#endif
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

    char *site_lib = opam_switch_lib(switch_name);
    log_debug("site-lib: %s", site_lib);

    char *ocaml_version = opam_switch_ocaml_version(switch_name);
    log_debug("ocaml_version: %s", ocaml_version);

    char *compiler_version = opam_switch_base_compiler_version(switch_name);
    log_debug("base_compiler_version: %s", compiler_version);

    log_debug("config_test exiting");
}
