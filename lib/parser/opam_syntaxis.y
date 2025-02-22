/* fvf = filtered version formula
   fpf = filtered package formula */

%include {
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utarray.h"
#include "uthash.h"

#include "liblogc.h"

#if INTERFACE
#ifndef YYMALLOCARGTYPE
#define YYMALLOCARGTYPE size_t
#endif
#endif

#define DEBUG_LEVEL opamc_syntaxis_debug
extern int  DEBUG_LEVEL;
#define TRACE_FLAG opamc_syntaxis_trace
extern bool TRACE_FLAG;
}

/* %ifdef PROFILE_dev */
/* %endif */

/* opam_parse_state->pkg->entries is a uthash of bindings. parse rules
update it directly */
/* a binding is (name, val); val may be another hash (e.g. for depends: key)  */
/* FIXME: we need a custom struct for deps: name, version, optional version_relop, optional constraint (e.g. 'with-test') */
%extra_argument { struct opam_parse_state_s *opam_parse_state }
/* %extra_argument { struct opam_package_s *opam_pkg } */

/* we piggyback on lemon to declare some type consts */
/* %token BINDING_BOOL. */
/* %token BINDING_INT. */
/* %token BINDING_STRING. */
/* %token BINDING_STRINGLIST. */

%token AMP.
%token AUTHORS.
%token AVAILABLE.
%token BANG.
%token BUG_REPORTS.
%token BOOL.
%token BUILD.
%token BUILD_DOC.
%token BUILD_ENV.
%token COLON.
%token COMMA.
%token CONFLICTS.
%token CONFLICT_CLASS.
%token DEPENDS.
%token DEPEXTS.
%token DEPOPTS.
%token DESCRIPTION.
%token DEV_REPO.
%token DOC.
%token DQ.
%token EQ.
%token ERROR.
%token EXTRA_FILES.
%token EXTRA_SOURCE.
%token FALSE.
%token FEATURES.
%token FILTER.
%token FLAGS.
%token FVF_LOGOP.
%token HOMEPAGE.
%token IDENT.
%token IDENTCHAR.
%token INSTALL.
%token KEYWORD.
%token LBRACE.
%token LBRACKET.
%token LICENSE.
%token LOGOP.
%token LPAREN.
%token MAINTAINER.
%token MESSAGES.
%token OPAM_VERSION.
%token PACKAGE.
%token PATCHES.
%token PIN_DEPENDS.
%token PKGNAME.
%token POST_MESSAGES.
%token QMARK.
%token RBRACE.
%token RBRACKET.
%token REMOVE.
%token RELOP.
%token RPAREN.
%token RUN_TEST.
%token SETENV.
%token SQ.
%token STRING.
%token STRING3.
%token SUBSTS.
%token SYNOPSIS.
%token TAGS.
%token TERM.
%token TERM_STRING.
%token TERM_VARIDENT.
%token TQ.
%token TRUE.
%token URL.
%token VARIDENT.
%token VERSION.

%right LOGOP.
%right BANG.
%right QMARK.

%right RELOP.


/* **************** */
%token_type { union opam_token_u } /* terminals */

/* %default_type { struct opam_entry_s * } /\* non-terminal default *\/ */

%type package { struct opam_package_s * }
%type binding { struct opam_binding_s * }
%type fpf { struct opam_deps_s * }
%type stringlist { UT_array * }

%syntax_error {
    log_error("**************** SYNTAX ERROR! ****************");
/* args passed:
   yyParser *yypParser, /\* The parser *\/
   int yymajor, /\* The major type of the error token *\/
   ParseTOKENTYPE yyminor /\* The minor type of the error token *\/
   ParseTOKENTYPE is union opam_token_u
 */


%ifdef YYDEBUG_EXIT_ON_ERROR
   exit(EXIT_FAILURE);
%else
     log_error("trying to recover...");
%endif
}

%parse_accept {
    LOG_TRACE(0, "Parsing complete", "");
}

%parse_failure {
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
}

/* *******************  PARSE RULES  ********************* */
%start_symbol package

package ::= bindings . {
#if YYDEBUG
    LOG_DEBUG(0, "PACKAGE", "");
    LOG_DEBUG(0, " bindings ct: %d",
              HASH_CNT(hh, opam_parse_state->pkg->entries));
#endif
}

%ifdef YYDEBUG_BUILD_FILTER
package ::= build_filter . {
    LOG_DEBUG(0, "START build_filter", "");
}
%endif

%ifdef YYDEBUG_FILTER
package ::= filter . {
    LOG_DEBUG(0, "START filter", "");
}
%endif

%ifdef YYDEBUG_FPF
package ::= filtered_package_formulas . {
    LOG_DEBUG(0, "START filtered_package_formulas");
}
%endif

%ifdef YYDEBUG_FVF
package ::= fvf_expr . {
    LOG_DEBUG(0, "START fvf");
    /* printf("    bindings ct: %d\n", */
    /*        HASH_CNT(hh, opam_parse_state->pkg->entries)); */
}
%endif

 /****************************************************************/
bindings ::= binding . {
/* #if YYDEBUG */
/*     printf("ONE BINDING\n"); */
/* #endif */
}

bindings(Bindings_lhs) ::= bindings(Bindings_rhs) binding . {
#if YYDEBUG
    LOG_DEBUG(0, "BINDINGS ct: %d",
           HASH_CNT(hh, opam_parse_state->pkg->entries));
#endif
    /* no need to do anything, opam_parse_state->pkg->entries already
       contains all bindings */
    Bindings_lhs = Bindings_rhs;
}

/* <field-binding> ::= <ident> : <value> */
/* <value> ::= <bool> | <int> | <string> | <ident> | <varident> | <operator> | <list> | <option> | "(" <value> ")" */

binding(Binding) ::= KEYWORD(Keyword) COLON STRING(String) . {
#if YYDEBUG
    LOG_DEBUG(0, "BINDING: %s: %s", Keyword.s, String.s);
#endif
    /* create a binding and add it to the pkg hashmap */
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = Keyword.s;
    Binding->t = BINDING_STRING;
    Binding->val = strdup(String.s);
    HASH_ADD_KEYPTR(hh, // opam_parse_state->pkg->entries->hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
}

binding(Binding) ::= KEYWORD(Keyword) COLON string3(String) . {
#if YYDEBUG
    LOG_DEBUG(0, "BINDING: %s: %s", Keyword.s, String.s);
#endif
    /* create a binding and add it to the pkg hashmap */
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = Keyword.s;
    Binding->t = BINDING_STRING;
    Binding->val = strdup(String.s);
    HASH_ADD_KEYPTR(hh, // opam_parse_state->pkg->entries->hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
}

binding(Binding) ::= KEYWORD(Keyword) COLON LBRACKET stringlist(Stringlist) RBRACKET . {
#if YYDEBUG
    LOG_DEBUG(0, "BINDING stringlist %s, ct: %d",
           Keyword.s, utarray_len(Stringlist));
#endif
    /* create a binding and add it to the pkg hashmap */
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = Keyword.s;
    Binding->t = BINDING_STRINGLIST;
    Binding->val = (void*)Stringlist;
    HASH_ADD_KEYPTR(hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
#if YYDEBUG
    LOG_DEBUG(0, "BINDING stringlist val ct: %d",
           utarray_len((UT_array*)Binding->val));
#endif
}

/****************************************************************/
/* build, install, run-test all have the same grammar:
    [ [ <term> { <filter> } ... ] { <filter> } ... ]
 */
binding(Binding) ::=
    BUILD COLON LBRACKET term_grammar RBRACKET . {
#if YYDEBUG
        LOG_DEBUG(0, "BINDING build", "");
#endif
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = strndup("build", 5);
    Binding->t = BINDING_BUILD;
    /* Binding->val =  */
    HASH_ADD_KEYPTR(hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
}

binding(Binding) ::=
    INSTALL COLON LBRACKET term_grammar RBRACKET . {
#if YYDEBUG
        LOG_DEBUG(0, "BINDING install", "");
#endif
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = strndup("install", 7);
    Binding->t = BINDING_BUILD;
    /* Binding->val =  */
    HASH_ADD_KEYPTR(hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
}

binding(Binding) ::=
    RUN_TEST COLON LBRACKET term_grammar RBRACKET . {
#if YYDEBUG
        LOG_DEBUG(0, "BINDING run-test", "");
#endif
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = strndup("run-test", 8);
    Binding->t = BINDING_BUILD;
    /* Binding->val =  */
    HASH_ADD_KEYPTR(hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
}

term_grammar ::= . {
    LOG_DEBUG(0, "term_grammar null", "");
}

term_grammar ::= term_grammar build_cmd . {
    LOG_DEBUG(0, "term_grammar list", "");
}

build_cmd ::= LBRACKET build_terms RBRACKET . {
    LOG_DEBUG(0, "build_cmd", "");
}

build_cmd ::= LBRACKET build_terms RBRACKET build_filter . {
    LOG_DEBUG(0, "build_cmd filtered", "");
}

build_terms ::= . {
    LOG_DEBUG(0, "build_terms", "");
}

/* build_terms ::= build_terms TERM_STRING . { */
/*     LOG_DEBUG(0, "build_terms TERM_STRING"); */
/* } */

build_terms ::= build_terms build_term . {
    LOG_DEBUG(0, "build_terms build_term", "");
}

build_term ::= TERM_STRING . {
    LOG_DEBUG(0, "build_term TERM_VARIDENT", "");
}

build_term ::= TERM_STRING build_filter . {
    LOG_DEBUG(0, "build_term TERM_STRING build_term", "");
}

build_term ::= TERM_VARIDENT . {
    LOG_DEBUG(0, "build_term TERM_VARIDENT", "");
}

build_term ::= TERM_VARIDENT build_filter . {
    LOG_DEBUG(0, "build_term TERM_VARIDENT build_term", "");
}

build_filter ::= LBRACE FILTER RBRACE . {
    LOG_DEBUG(0, "build_filter", "");
}

build_filter ::= LBRACE build_filter_expr RBRACE . {
    LOG_DEBUG(0, "build_filter", "");
}

build_filter_expr ::= STRING . {
    LOG_DEBUG(0, "build_filter", "");
}

build_filter_expr ::= VARIDENT . {
    LOG_DEBUG(0, "build_filter", "");
}

build_filter_expr ::= build_filter_expr LOGOP build_filter_expr . {
    LOG_DEBUG(0, "build_filter", "");
}

build_filter_expr ::= build_filter_expr RELOP build_filter_expr . {
    LOG_DEBUG(0, "build_filter", "");
}

/* build_filter_expr ::= FILTER . { */
/*     LOG_DEBUG(0, "build_filter"); */
/* } */

/****************************************************************/
// depends and conflicts use same content model
//    [ <filtered-package-formula> ... ]
binding(Binding) ::=
    DEPENDS COLON LBRACKET filtered_package_formulas RBRACKET . {
#if YYDEBUG
        LOG_DEBUG(0, "BINDING depends", "");
#endif
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = strndup("depends", 7);
    Binding->t = BINDING_DEPENDS;
    /* Binding->val = ... will be list of pkgs */
    /* so here, alloc a UT hash table to hold the list */
    HASH_ADD_KEYPTR(hh,
                    opam_parse_state->pkg->entries,
                    Binding->name,
                    strlen(Binding->name),
                    Binding);
}

binding(Binding) ::=
    CONFLICTS COLON LBRACKET filtered_package_formulas RBRACKET . {
#if YYDEBUG
        LOG_DEBUG(0, "BINDING conflicts", "");
#endif
    Binding = calloc(sizeof(struct opam_binding_s), 1);
    Binding->name = strndup("conflicts", 9);
    Binding->t = BINDING_DEPENDS;
    /* Binding->val =  */
    HASH_ADD_KEYPTR(hh,
                    opam_parse_state->pkg->entries,
                    Binding->name, strlen(Binding->name), Binding);
}

stringlist(Stringlist) ::= STRING(String) . {
#if YYDEBUG
    LOG_DEBUG(0, "STRINGLIST single: %s", String.s);
#endif
    utarray_new(Stringlist, &ut_str_icd);
    utarray_push_back(Stringlist, &String.s);
}

stringlist(Stringlist_lhs) ::= stringlist(Stringlist) STRING(String) . {
#if YYDEBUG
    LOG_DEBUG(0, "STRINGLIST multiple, ct: %d; new: %s",
           utarray_len(Stringlist), String.s);
#endif
    utarray_push_back(Stringlist, &String.s);
    Stringlist_lhs = Stringlist;
}

/* depends: [ <filtered-package-formula> ... ]
   <filtered-package-formula> ::=
   <filtered-package-formula> <logop> <filtered-package-formula>
   | ( <filtered-package-formula> )
   | <pkgname> { { <filtered-version-formula> }* }

    <pkgname>         ::= (") <ident> (")
*/
/*     pkgname ::= DQ IDENT DQ . { */
/* #if YYDEBUG */
/*         printf("pkgname\n"); */
/* #endif */
/*     } */

    filtered_package_formulas ::= . {
#if YYDEBUG
        LOG_DEBUG(0, "filtered_package_formulas: null", "");
#endif
    }

    filtered_package_formulas ::= filtered_package_formulas fpf . {
#if YYDEBUG
        LOG_DEBUG(0, "filtered_package_formulas leftrec", "");
#endif
    }

    fpf ::= fpf LOGOP fpf . {
#if YYDEBUG
        LOG_DEBUG(0, "fpf: fpf LOGOP fpf", "");
#endif
    }

    fpf ::= LPAREN fpf RPAREN . {
#if YYDEBUG
        LOG_DEBUG(0, "fpf: (fpf)", "");
#endif
    }

    fpf ::= PKGNAME(Pkg) fvf_expr . {
        LOG_DEBUG(1, "fpf: pkgname fvf_expr:**************** %s",
                  Pkg.s);
#if YYDEBUG
#endif
    }

    fpf ::= PKGNAME(Pkg) . {
#if YYDEBUG
        LOG_DEBUG(0, "fpf: **************** pkgname: %s", Pkg.s);
#endif
        /* Deps = calloc(sizeof(struct opam_binding_s), 1); */
        /*  */
    }

    // ################################################################
        fvf_expr ::= LBRACE fvf RBRACE . {
#if YYDEBUG
        LOG_DEBUG(0, "fvf_expr: braces", "");
#endif
    }
/*
   <filtered-version-formula> ::=
   <filtered-version-formula> <logop> <filtered-version-formula>
   | "!" <filtered-version-formula>
   | "?" <filtered-version-formula>
   | "(" <filtered-version-formula> ")"
   | <relop> <version>
   | <filter>
   | <relop> <filter>

   e.g.   "ocaml" {>= "4.04.0"}
*/

    fvf ::= . {
#if YYDEBUG
        LOG_DEBUG(0, "fvf: null", "");
#endif
    }

        fvf ::= fvf logop_fvf . {
#if YYDEBUG
        LOG_DEBUG(0, "fvf: fvf logop_fvf", "");
#endif
    }

        logop_fvf ::= LOGOP fvf . {
#if YYDEBUG
        LOG_DEBUG(0, "fvf: fvf logop fvf", "");
#endif
    }

/*         fvf ::=  BANG fvf . { */
/* #if YYDEBUG */
/*         LOG_DEBUG(0, "fvf: bang fvf"); */
/* #endif */
/*     } */

/*         fvf ::= QMARK fvf . { */
/* #if YYDEBUG */
/*         LOG_DEBUG(0, "fvf: qmark fvf"); */
/* #endif */
/*     } */

// | "(" <filtered-version-formula> ")"
/*         fvf ::=  LPAREN fvf RPAREN . { */
/* #if YYDEBUG */
/*         LOG_DEBUG(0, "fvf: (fvf)"); */
/* #endif */
/*     } */

        // | <relop> <version>
/*         fvf_base ::= RELOP VERSION . { */
/* #if YYDEBUG */
/*         LOG_DEBUG(0, "fvf_base: relop version"); */
/* #endif */
/*     } */

       fvf ::= fvf_base . {
#if YYDEBUG
        LOG_DEBUG(0, "fvf: fvf_base", "");
#endif
       }

       fvf ::= filter_expr . [AMP] {
#if YYDEBUG
        LOG_DEBUG(0, "fvf: filter_expr", "");
#endif
       }

            fvf_base ::= VERSION . { // treated as <string>
#if YYDEBUG
        LOG_DEBUG(0, "fvf_base: version", "");
#endif
    }
        // | <filter>
/*         fvf_base ::= filter . { */
/* #if YYDEBUG */
/*             LOG_DEBUG(0, "fvf_base: filter"); */
/* #endif */
/*     } */

        // | <relop> <filter>
/*         fvf_base ::= RELOP filter . { */
/* #if YYDEBUG */
/*         LOG_DEBUG(0, "fvf_base: relop filter"); */
/* #endif */
/*     } */

    /*
   <filter> ::= <filter> <logop> <filter>
   | "!" <filter>
   | "?" <filter>
   | ( <filter> )
   | <filter> <relop> <filter>
   | <varident>
   | <string>
   | <int>
   | <bool>

   NB: <varident>, <string>, <int>, <bool> are lexed as FILTER tokens
    */

filter_expr ::= filter . [LOGOP] {
#if YYDEBUG
        LOG_DEBUG(0, "filter_expr: filter", "");
#endif
    }

            filter_expr ::= filter LOGOP filter . {
#if YYDEBUG
        LOG_DEBUG(0, "filter_expr: filter_expr logop_filter", "");
#endif
    }

        filter ::= RELOP VERSION . {
#if YYDEBUG
        LOG_DEBUG(0, "filter: relop version", "");
#endif
    }

        filter ::= RELOP filter . {
#if YYDEBUG
        LOG_DEBUG(0, "filter: relop filter", "");
#endif
    }

/*         logop_filter ::= LOGOP filter . { */
/* #if YYDEBUG */
/*         LOG_DEBUG(0, "logop_filter: logop filter"); */
/* #endif */
/*     } */

filter ::= LPAREN filter RPAREN . {
#if YYDEBUG
        LOG_DEBUG(0, "filter: (filter)", "");
#endif
    }

    filter ::= BANG filter . {
#if YYDEBUG
        LOG_DEBUG(0, "filter: BANG filter", "");
#endif
    }

    filter ::= QMARK filter . {
#if YYDEBUG
        LOG_DEBUG(0, "filter: QMARK filter", "");
#endif
    }

    filter ::= FILTER . { // varident|string|int|bool
#if YYDEBUG
        LOG_DEBUG(0, "filter: FILTER", "");
#endif
    }

filter ::= VARIDENT . {
#if YYDEBUG
        LOG_DEBUG(0, "filter: VARIDENT", "");
#endif
}

string3 ::= TQ STRING3 TQ . {
}

string3 ::= TQ STRING3 STRING STRING3 TQ . {
}
