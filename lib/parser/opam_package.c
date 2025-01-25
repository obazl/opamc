/* types etc. for parsed opam files */

#include "opam_package.h"

#define DEBUG_LEVEL debug_opamc_syntaxis
extern int  DEBUG_LEVEL;
#define TRACE_FLAG trace_opamc_syntaxis
extern bool TRACE_FLAG;

#if EXPORT_INTERFACE
enum phrase_type_e {
    BINDING_BOOL,
    BINDING_BUILD,
    BINDING_DEPENDS,
    BINDING_INT,
    BINDING_STRING,
    BINDING_STRINGLIST,
};

struct opam_package_s {
    const char *src;
    struct opam_binding_s *bindings; /* uthash table */
};

#include "uthash.h"
struct opam_binding_s {
    char *name;                    /* key */
    // WARNING: if we use 'int t', then the enum above will not be
    // included by makeheaders.
    enum phrase_type_e t;
    void *val;
    UT_hash_handle hh; /* makes this structure hashable */
};

/* build: [ [ <term> {filter} ...] {filter} ... ] */
/*   buildcmds ==  [ (cmd filter?)+ ] */
/*   cmd == arg+, where arg == term | filter */
#include "utarray.h"

struct opam_cmd_s {
    UT_array *args;  /* array of opam_arg_s */
    char *filter;
};

struct opam_arg_s {
    int  t;  // 0=string, 1=var, 2=filter?
    char *val;
};

/* for utarray: */
#include <stdio.h>
#endif

UT_icd opam_cmd_icd = {sizeof(struct opam_cmd_s),
                        NULL, NULL, NULL};
UT_icd opam_arg_icd = {sizeof(struct opam_arg_s),
                       NULL, NULL, NULL};

EXPORT void opam_package_free(struct opam_package_s *pkg)
{
    (void)pkg;
    printf("opam_package_free\n");
    /* iterate over entries */
}
