#include <stdlib.h>

#include "liblogc.h"
#include "unity.h"
#include "lexer_test.h"

char *errmsg = "";

char *data, *actual, *expected;

#define DEBUG_LEVEL debug_opamc_test
extern int  DEBUG_LEVEL;
#define TRACE_FLAG trace_opamc_test
extern bool TRACE_FLAG;

extern int  debug_opamc_lexis;
extern bool trace_opamc_lexis;
extern int  debug_opamc_syntaxis;
extern bool trace_opamc_syntaxis;

void setUp(void) {
    if (fflush(stdout) == EOF) {
        /* Handle error */
    }
}

void tearDown(void) {
    if (fflush(stdout) == EOF) {
        /* Handle error */
    }
}

/* filtered-version-formula */
void fvf_simple(void) {
    data = "{ = \"3.1.2\" }";
    expected = "LBRACE RELOP='=' STRING=\"3.1.2\" RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);

    data = "{ ! = \"3.1.2\" }";
    expected = "LBRACE BANG RELOP='=' STRING=\"3.1.2\" RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);

    data = "{ ? = \"3.1.2\" }";
    expected = "LBRACE QMARK RELOP='=' STRING=\"3.1.2\" RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);

    data = "{ ( = \"3.1.2\" ) }";
    expected = "LBRACE LPAREN RELOP='=' STRING=\"3.1.2\" RPAREN RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);
}

void fvf_logop(void) {
    data = " depends: { = \"1.2.3\" }";
    /* data = "{ >= \"3.12\" & build }"; */
    expected = "LBRACE RELOP='>=' STRING=\"3.1.2\" LOGOP='&' RPAREN RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);
}

void filters(void) {
    data = " { \"foo\" }";
    expected = "LBRACE STRING=\"foo\" RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);

    data = "{ foo:installed }";
    expected = "LBRACE varident=\"foo\" COLON varident=\"installed\" RBRACE";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);
}

void dbg(void) {
    /* data = " \"foo\" { \"bar\" }"; */
    /*ok data = " \"foo\" { build }"; */
    /*ok? data = " \"foo\" { true }"; */
    //data = " \"foo\" { 32 }";
    /* data = " \"foo\" { \"3.2\" & build }"; */
    //data = " \"foo\" { >=\"3.12\" & build }";
    /* data = "build  : [ [\"dune\" \"subst\"] {pinned}"; */
    data = "build: [[foo] { 32 }]";
    expected = "";
    actual = opam_lex_string(data);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    free(actual);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    initialize(argc, argv, "lexis");
    LOG_DEBUG(0, "beginning lexer test", "");

    UNITY_BEGIN();

    /* **************** */
    /* RUN_TEST(filters); */
    /* RUN_TEST(fvf_simple); */
    /* RUN_TEST(fvf_logop); */
    /* RUN_TEST(fpf); */

    RUN_TEST(dbg);

    int rc = UNITY_END();
    if (fflush(stdout) == EOF) {
        /* Handle error */
    }
    return rc;
}
