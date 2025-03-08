/**
 * @file main.c
 * @brief A minimake tool like make
 * @date 2024-03-08
 * @author sanchuanheh
 * @version 0.1
 */

#include <argp.h>
#include <stdlib.h>
#include "loger.h"
#include "mk_parser.h"

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Enable verbose mode", 0},
    {"ersion", 'V', 0, 0, "Show version", 0},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    // 实际参数处理逻辑
    switch (key)
    {
    case 'v':
        log_info("verbose mode enabled");
        logger_config.level = LOG_DEBUG; // 启用 DEBUG 级别日志
        log_debug("Verbose mode activated");
        break;
    case 'V':
        printf("Version 0.1\n");
        break;
    case ARGP_KEY_ARG:
        // log_info("Argument: %s", arg);
        log_debug("Argument: %s", arg);
        int err = mk_parser(arg);
        log_debug("err: %d", err);
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1)
            argp_usage(state);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {
    options,
    parse_opt,
    "[TARGET]...",
    "A minimake tool like make",
    0, 0, 0};

int main(int argc, char **argv)
{
    argp_parse(&argp, argc, argv, 0, 0, 0);
    return 0;
}
