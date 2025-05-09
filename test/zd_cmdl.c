#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_DYNAMIC_ARRAY
#define ZD_DS_STRING
#define ZD_COMMAND_LINE
#include "zd.h"

/* ./zd_cmdl a b c -opt1 val1_1 val1_2 -opt2 -opt3 val3_1 -opt4 */

static struct zd_cmdl cmdl = {0};
int g_argc = 0;
char **g_argv = NULL;

static char *test1(void)
{
    zd_assert(strcmp(cmdl.program.buf, g_argv[0]) == 0, "cmdl program");
    return "test cmdl program done!";
}

static char *test2(void)
{
    zd_assert(cmdl.count == 11, "cmdl count");
    return "test cmdl count done!";
}

static char *test3(void)
{
    zd_assert(cmdl.nopts.count == 3, "cmdl non-opts count");

    struct zd_string *p_string;
    p_string = (struct zd_string *) zd_dyna_get(&cmdl.nopts, 0); zd_assert(strcmp(p_string->buf, "a") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&cmdl.nopts, 1); zd_assert(strcmp(p_string->buf, "b") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&cmdl.nopts, 2); zd_assert(strcmp(p_string->buf, "c") == 0, NULL);

    return "test cmdl non-opts done!";
}

static char *test4(void)
{
    zd_assert(cmdl.opts.count == 4, "cmdl opts count");

    struct zd_cmdlopt *p_cmdlopt;
    struct zd_string *p_string;

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 0); zd_assert(strcmp(p_cmdlopt->name.buf, "-opt1") == 0, "test opt1");
    zd_assert(p_cmdlopt->vals.count == 2, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(strcmp(p_string->buf, "val1_1") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 1); zd_assert(strcmp(p_string->buf, "val1_2") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 2); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 1); zd_assert(strcmp(p_cmdlopt->name.buf, "-opt2") == 0, "test opt2");
    zd_assert(p_cmdlopt->vals.count == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 2); zd_assert(strcmp(p_cmdlopt->name.buf, "-opt3") == 0, "test opt3");
    zd_assert(p_cmdlopt->vals.count == 1, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(strcmp(p_string->buf, "val3_1") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 1); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 3); zd_assert(strcmp(p_cmdlopt->name.buf, "-opt4") == 0, "test opt4");
    zd_assert(p_cmdlopt->vals.count == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 2); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 4); zd_assert(p_cmdlopt == NULL, NULL);

    return "test cmdl opts done!";
}

static char *test5(void)
{
    int is_valid = 0;
    struct zd_dyna vals = zd_cmdl_getopt(&cmdl, "-opt1", &is_valid);
    zd_assert(is_valid == 1, NULL);
    zd_assert(vals.count == 2, NULL);

    struct zd_string *p_string;
    p_string = (struct zd_string *) zd_dyna_get(&vals, 0); zd_assert(strcmp(p_string->buf, "val1_1") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&vals, 1); zd_assert(strcmp(p_string->buf, "val1_2") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&vals, 2); zd_assert(p_string == NULL, NULL);


    vals = zd_cmdl_getopt(&cmdl, "-opt6", &is_valid);
    zd_assert(is_valid == 0, NULL);
    zd_assert(vals.count == 0, NULL);

    return "test cmdl getopt done!";
}

int main(int argc, char **argv)
{
    g_argc = argc;
    g_argv = argv;
    zd_cmdl_build(&cmdl, argc, argv);

    struct zd_testsuite suite = {0};
    suite.name = "test_cmdl";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_run_test(&suite, test3);
    zd_run_test(&suite, test4);
    zd_run_test(&suite, test5);
    zd_test_summary(&suite);

    zd_cmdl_destroy(&cmdl);
 
    return 0;
}
