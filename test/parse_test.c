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
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

#include "liblogc.h"
#include "utarray.h"
#include "utstring.h"
#include "opamc.h"

#include "parse_test.h"

UT_string *build_file;

UT_string *buffer;

/* #define DEBUG_LEVEL debug_opamc_syntaxis */
extern  int  debug_opamc_syntaxis;
extern  int  debug_opamc_lexis;
/* #define TRACE_FLAG trace_opamc_syntaxis */
extern  bool  trace_opamc_syntaxis;

int compareFiles(FILE *file1, FILE *file2)
{
    /* printf("comparing files\n"); */
    char ch1 = getc(file1);
    char ch2 = getc(file2);
    int error = 0;
    (void)error;
    int pos = 0, line = 1;
    while (ch1 != EOF && ch2 != EOF){
        pos++;
        if (ch1 == '\n' && ch2 == '\n'){
            line++;
            pos = 0;
        }
        if (ch1 != ch2){
            error++;
            printf("File mismatch at (%d:%d)\n", line, pos);
            return -1;
        }
        ch1 = getc(file1);
        ch2 = getc(file2);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int opt;

    /* opamc_trace = true; */
    /* opamc_debug = 2; */

    debug_opamc_syntaxis = 1;
    trace_opamc_syntaxis = true;
    debug_opamc_lexis    = 1;

    utstring_new(build_file);

    while ((opt = getopt(argc, argv, "f:hv")) != -1) {
        switch (opt) {
        case 'f':
            /* BUILD.bazel or BUILD file */
            utstring_printf(build_file, "%s", optarg);
            break;
        case 'h':
            log_info("Help: ");
            exit(EXIT_SUCCESS);
        default:
            log_error("Usage: %s -f /path/to/file.opam", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (utstring_len(build_file) == 0) {
        log_error("-- -f /path/to/file.opam must be provided.");
        exit(EXIT_FAILURE);
    }

    char *wd = getenv("BUILD_WORKING_DIRECTORY");
    if (wd) {
        /* we launched from bazel workspace, cd to launch dir */
        chdir(wd);
    }

    struct opam_package_s
        *opam_pkg = opam_parse_file(utstring_body(build_file));

    printf("opam pkg: %s\n", opam_pkg->fname);
    struct opam_binding_s *binding;

    for (binding = opam_pkg->entries;
         binding != NULL;
         binding = binding->hh.next) {
        switch (binding->t) {
        case BINDING_STRING:
            printf("binding: %s: %s\n", binding->name, (char*)binding->val);
            break;
        case BINDING_STRINGLIST: {
            UT_array *strings = (UT_array*)binding->val;
            printf("Binding: %s: <stringlist> ct: %d\n",
                   binding->name, utarray_len(strings));
            char **p = NULL;
            while ( (p=(char**)utarray_next(strings, p))) {
                printf("\t%s\n", *p);
            }
        }
            break;
        case BINDING_DEPENDS: {
            log_info("DEPS");
            printf("binding: %s: %s\n", binding->name, (char*)binding->val);
            /* UT_array *strings = (UT_array*)binding->val; */
            /* printf("Binding: %s: <stringlist> ct: %d\n", */
            /*        binding->name, utarray_len(strings)); */
            /* char **p = NULL; */
            /* while ( (p=(char**)utarray_next(strings, p))) { */
            /*     printf("\t%s\n", *p); */
            /* } */
        }
            break;
        default:
            printf("binding: %s: %s\n", binding->name, (char*)binding->val);
        }
    }
}
