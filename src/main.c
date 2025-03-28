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
#include "mk_runner.h"
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Enable verbose mode", 0},
    {"ersion", 'V', 0, 0, "Show version", 0},
    {0}};
MkTarget_p targets = NULL;
igraph_t graph;
int targetNum = 0;
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  // 实际参数处理逻辑
  switch (key) {
    case 'v':
      LogInfo("verbose mode enabled");
      logger_config.level = LOG_DEBUG;  // 启用 DEBUG 级别日志
      int err = MkClean();
      if (err != 0) {
        LogError("MkClean failed");
        return err;
      }
      LogDebug("Verbose mode activated");
      break;
    case 'V':
      printf("Version 0.1\n");
      break;
    case ARGP_KEY_ARG:
      // LogInfo("Argument: %s", arg);
      LogDebug("Argument: %s", arg);
      // int ret = MkRun(&targets[0]);
      // if (ret != 0) {
      //   LogError("MkRun failed");
      //   return ret;
      // }
      int ret = MkRunFrom(targets, targetNum, arg, &graph);
      if (ret != 0) {
        LogError("ret: %d", ret);
        LogError("arg MkRunFrom failed: %s", arg);
        return ret;
      }
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) argp_usage(state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {
    options, parse_opt, "[TARGET]...", "A minimake tool like make", 0, 0, 0};

int main(int argc, char **argv) {
  // logger_config.level = LOG_DEBUG;
  targetNum = MkParser(&targets);
  LogDebug("targetNum: %d", targetNum);
  if (targetNum == -1) {
    LogError("MkParser failed");
    return -1;
  } else if (targetNum == 0) {
    LogError("No target found");
    return -1;
  }
  MkDepCheck(&targets, targetNum);
  MkTargetCheck(&targets, targetNum);

  int ret = MkGraphInit(&graph, &targets, targetNum);
  if (ret != 0) {
    LogError("MkGraphInit failed");
    return -1;
  }
  argp_parse(&argp, argc, argv, 0, 0, 0);
  FreeMkTargets(&targets, targetNum);
  return 0;
}
