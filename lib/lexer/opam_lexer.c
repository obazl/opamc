#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

#include "liblogc.h"

#include "opam_lexer.h"

#define DEBUG_LEVEL opamc_lexis_debug
int  DEBUG_LEVEL;
#define TRACE_FLAG opamc_lexis_trace
bool TRACE_FLAG;

#if EXPORT_INTERFACE
#define TOKEN_NAME(x) (char*)#x
#endif

#if defined(LEXDEBUG)
EXPORT char *opam_token_names[256] = {
    [AMP]      = TOKEN_NAME(amp),
    [AUTHORS]      = TOKEN_NAME(authors),
    [AVAILABLE]      = TOKEN_NAME(available),
    [BANG]      = TOKEN_NAME(bang),
    [BOOL]      = TOKEN_NAME(bool),
    [BUG_REPORTS]  = TOKEN_NAME(bug_reports),
    [BUILD]      = TOKEN_NAME(build),
    [BUILD_DOC]      = TOKEN_NAME(build_doc),
    [BUILD_ENV]      = TOKEN_NAME(build_env),
    [COLON]        = TOKEN_NAME(colon),
    [COMMA]        = TOKEN_NAME(comma),
    [CONFLICTS]      = TOKEN_NAME(conflicts),
    [CONFLICT_CLASS]      = TOKEN_NAME(conflict_class),
    [DEPENDS]      = TOKEN_NAME(depends),
    [DEPEXTS]      = TOKEN_NAME(depexts),
    [DEPOPTS]      = TOKEN_NAME(depopts),
    [DESCRIPTION]      = TOKEN_NAME(description),
    [DEV_REPO]      = TOKEN_NAME(dev_repo),
    [DOC]          = TOKEN_NAME(doc),
    [DQ]           = TOKEN_NAME(dq),
    [EQ]           = TOKEN_NAME(eq),
    [ERROR]        = TOKEN_NAME(error),
    [EXTRA_FILES]      = TOKEN_NAME(extra_files),
    [EXTRA_SOURCE]      = TOKEN_NAME(extra_source),
    [FALSE]        = TOKEN_NAME(FALSE),
    [FEATURES]      = TOKEN_NAME(features),
    [FILTER]      = TOKEN_NAME(filter),
    [FLAGS]      = TOKEN_NAME(flags),
    /* [FVF_LOGOP]      = TOKEN_NAME(FVF_LOGOP), */
    [HOMEPAGE]     = TOKEN_NAME(homepage),
    [IDENT]      = TOKEN_NAME(ident),
    [IDENTCHAR]      = TOKEN_NAME(identchar),
    [INSTALL]      = TOKEN_NAME(install),
    [KEYWORD]      = TOKEN_NAME(keyword),
    [LBRACE]      = TOKEN_NAME(lbrace),
    [LBRACKET]      = TOKEN_NAME(lbracket),
    [LICENSE]      = TOKEN_NAME(license),
    [LOGOP]      = TOKEN_NAME(LOGOP),
    [LPAREN]       = TOKEN_NAME(lparen),
    [MAINTAINER]   = TOKEN_NAME(maintainer),
    [MESSAGES]      = TOKEN_NAME(messages),
    [OPAM_VERSION] = TOKEN_NAME(opam_version),
    [PACKAGE]      = TOKEN_NAME(package),
    [PATCHES]      = TOKEN_NAME(patches),
    [PIN_DEPENDS]      = TOKEN_NAME(pin_depends),
    [PKGNAME]      = TOKEN_NAME(pkgname),
    [POST_MESSAGES]      = TOKEN_NAME(post_messages),
    [QMARK]      = TOKEN_NAME(qmark),
    [RBRACE]      = TOKEN_NAME(rbrace),
    [RBRACKET]      = TOKEN_NAME(rbracket),
    [RELOP]      = TOKEN_NAME(RELOP),
    [REMOVE]      = TOKEN_NAME(remove),
    [RPAREN]       = TOKEN_NAME(rparen),
    [RUN_TEST]      = TOKEN_NAME(run_test),
    [SETENV]      = TOKEN_NAME(setenv),
    [SQ]      = TOKEN_NAME(sq),
    [STRING]       = TOKEN_NAME(string),
    [STRING3]       = TOKEN_NAME(string3),
    [SUBSTS]      = TOKEN_NAME(substs),
    [SYNOPSIS]      = TOKEN_NAME(synopsis),
    [TAGS]      = TOKEN_NAME(tags),
    [TERM]      = TOKEN_NAME(TERM),
    [TERM_STRING]      = TOKEN_NAME(TERM_STRING),
    [TERM_VARIDENT]      = TOKEN_NAME(TERM_VARIDENT),
    [TQ]         = TOKEN_NAME(TQ),
    [TRUE]         = TOKEN_NAME(TRUE),
    [URL]      = TOKEN_NAME(url),
    [VARIDENT]     = TOKEN_NAME(varident),
    [VERSION]      = TOKEN_NAME(version),
    NULL
};
#endif

bool is_empty(const char *s)
{
  while (*s) {
    if (!isspace(*s))
      return false;
    s++;
  }
  return true;
}

/* opam_lex_file - for testing TODO: move to test/lex_test.c? */
/* see sealark_parsers.c */
/* EXPORT UT_array *opam_lex_file(char *fname) */
EXPORT void opam_lex_file(char *fname)
{
    TRACE_ENTRY;
    log_set_quiet(false);

    /* UT_array *token_list; */
    /* utarray_new(token_list, &node_icd); */
/* nodelist: UT_array of node_s */
// sealark_nodes.c:
/* EXPORT UT_icd node_icd = {sizeof(struct node_s), NULL, sealark_alias_node, sealark_node_free}; */


    /* log_info("_lex_file: %s", fname); */
    FILE *f;

    f = fopen(fname, "r");
    if (f == NULL) {
        perror(fname);
        log_error("fopen failure for %s", fname);
        /* log_error("Value of errno: %d", errnum); */
        /* log_error("fopen error %s", strerror( errnum )); */
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    const size_t fsize = (size_t) ftell(f);
    if (fsize == 0) {
        fclose(f);
        errno = -1;
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_SET);
    char *buffer = (char*) malloc(fsize + 1);
    size_t read_ct = fread(buffer, 1, fsize, f);
    (void)read_ct; //FIXME: error check
    buffer[fsize] = 0;
    fclose(f);

    if (is_empty(buffer)) {
        fclose(f);
        errno = -2;
        exit(EXIT_FAILURE);
    }

    struct opam_lexer_s * lexer = malloc(sizeof(struct opam_lexer_s));
    opam_lexer_init(fname, lexer, buffer);
    /* struct bf_lexer_s * lexer = malloc(sizeof(struct bf_lexer_s)); */
    /* lexer_init(fname, lexer, buffer); */

    int tok;
    union opam_token_u otok; // = malloc(sizeof(union opam_token));


    log_set_quiet(false);
    log_set_level(LOG_TRACE);
    /* log_info("starting lex"); */

    while ( (tok = get_next_opam_token(lexer, &otok)) != 0 ) {
#if defined(LEXDEBUG)
        LOG_DEBUG(0, "mode: %d; token type: %d: %s",
                  lexer->mode,
                  tok, opam_token_names[tok]);
        switch(tok) {
        case DESCRIPTION:
        case FILTER:
        case KEYWORD:
        case LOGOP:
        case OPAM_VERSION:
        case PKGNAME:
        case RELOP:
        case STRING:
        case STRING3:
        case SYNOPSIS:
        case TERM:
        case TERM_STRING:
        case TERM_VARIDENT:
        case VARIDENT:
        case VERSION:
            LOG_DEBUG(0, "\ts: %s", (char*)otok.s); break;
        /* default: */
        /*     LOG_DEBUG(0, "other: %d", tok); break; */
        }
#endif
        /* otok = malloc(sizeof(union opam_token)); */
    }
    LOG_TRACE(0, "opam_lexer: end of input", "");
    free(buffer);

    /* return token_list; */
    TRACE_EXIT;
}
