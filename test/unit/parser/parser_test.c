#include <stdlib.h>

#include "liblogc.h"
#include "unity.h"
#include "parser_test.h"

char *errmsg = "";

char *data, *actual, *expected;

struct opam_package_s *opkg;

#define DEBUG_LEVEL debug_opamc_test
extern int DEBUG_LEVEL;
extern int debug_opamc_lexis;
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

void string_flds(void) {
    data = "name: \"foo\"";
    LOG_DEBUG(1, "data: \"%s\"", data);
    opkg = opam_parse_string("string_flds", data);
    TEST_ASSERT_EQUAL_STRING("name", opkg->bindings->name);
    opam_package_free(opkg);
}

void build_fld(void) {
    expected = "build";

    data = "build: [[foo]]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = "build: [[foo] { \"bar\" }]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = "build: [[foo] { bar }]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = "build: [[foo] { 32 }]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = " build: [[foo] {bar}]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = " build: [[foo] {bar}]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = " build: [[foo {bar}] {baz}]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = "build: [[\"foo\"  \"-bar\" { baz }]]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);

    data = " build: [[foo] { \"3.2\" & build }]";
    opkg = opam_parse_string("build_fld", data);
    TEST_ASSERT_EQUAL_STRING(expected, opkg->bindings->name);
    opam_package_free(opkg);
    //data = " build: { >=\"3.12\" & build }";
    /* data = "build: [ [\"dune\" subst] ]"; */
    /* data = "build: [ [\"foo\" bar] {pinned | \"unpinned\" } ]"; */
}

void deps(void) {
    /*
      ("neat_package" | "also_neat_package") & "required_package"
      "foo" { >= "3.12" } & ("bar" | "baz" { !(> "2" & < "3.5") & != "5.1" })
    */
}

void case001(void) {
    data = "version: \"1.2.0\"\nbuild: [[\"bar\" \"baz\"]]";
    LOG_DEBUG(1, "data: \"%s\"", data);
    opkg = opam_parse_string("test_1", data);
    TEST_ASSERT_EQUAL_STRING("test_1", opkg->src);
    int ct = HASH_CNT(hh, opkg->bindings);
    TEST_ASSERT_EQUAL_INT(2, ct);
    struct opam_binding_s *b = NULL;
    char *key = "version";
    HASH_FIND_STR(opkg->bindings, key, b);
    if (b) {
        TEST_ASSERT_EQUAL_STRING("1.2.0", b->val);
    } else {
        UT_string *tmp; utstring_new(tmp);
        utstring_printf(tmp, "Binding %s not found.", key);
        TEST_FAIL_MESSAGE(utstring_body(tmp));
        utstring_free(tmp);
    }
    b = NULL;
    key = "build";
    HASH_FIND_STR(opkg->bindings, key, b);
    if (b) {
        UT_array *cmds = b->val;
        int cmd_ct = utarray_len(cmds);
        TEST_ASSERT_EQUAL_INT(1, cmd_ct);
        struct opam_cmd_s *cmd = utarray_front(cmds);
        struct opam_arg_s *arg;
        arg = utarray_eltptr(cmd->args, 0);
        TEST_ASSERT_EQUAL_STRING("bar", arg->val);
        arg = utarray_eltptr(cmd->args, 1);
        TEST_ASSERT_EQUAL_STRING("baz", arg->val);
        int arg_ct = utarray_len(cmd->args);
        TEST_ASSERT_EQUAL_INT(2, arg_ct);
    } else {
        UT_string *tmp; utstring_new(tmp);
        utstring_printf(tmp, "Binding %s not found.", key);
        TEST_FAIL_MESSAGE(utstring_body(tmp));
        utstring_free(tmp);
    }
    opam_package_free(opkg);
}

void dbg(void) {
    expected = "build";
    /* data = " \"foo\" { \"bar\" }"; */
    /*ok data = " \"foo\" { build }"; */
    /*ok? data = " \"foo\" { true }"; */

    /* data = "build: [[foo bar]]"; */
    //data = "build: [[foo] { \"bar\" }]";

    data = "build: [[\"foo\" \"bar\"]]";
    data = "version: \"1.2.0\"\nbuild: [[\"bar\" \"baz\"]]";
    LOG_DEBUG(1, "data: \"%s\"", data);
    opkg = opam_parse_string("test_1", data);
    int ct = HASH_CNT(hh, opkg->bindings);
    TEST_ASSERT_EQUAL_INT(2, ct);
    struct opam_binding_s *b;
    char *key = "version";
    HASH_FIND_STR(opkg->bindings, key, b);
    if (b) {
        LOG_DEBUG(0, "found %s", key);
        TEST_ASSERT_EQUAL_STRING("1.2.0", b->val);

    } else {
        LOG_DEBUG(0, "not found: %s", key);
    }
    TEST_ASSERT_EQUAL_STRING("test_1", opkg->src);
    opam_package_free(opkg);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    initialize(argc, argv, "parser");
    LOG_DEBUG(0, "beginning parser test", "");

    UNITY_BEGIN();

    /* RUN_TEST(string_flds); */
    /* RUN_TEST(build_fld); */
    /* RUN_TEST(filters); */
    /* RUN_TEST(fvf_simple); */
    /* RUN_TEST(fvf_logop); */
    /* RUN_TEST(fpf); */

    RUN_TEST(case001);
    /* RUN_TEST(dbg); */

    int rc = UNITY_END();
    if (fflush(stdout) == EOF) {
        /* Handle error */
    }
    return rc;
}
