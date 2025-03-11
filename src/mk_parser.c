#include "mk_parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loger.h"

int MkClean() {
  if (logger_config.level < LOG_DEBUG) {
    return 0;
  }

  FILE *fp = fopen("Makefile", "r");
  if (fp == NULL) {
    LogError("Makefile not found");
    return -1;
  }
  char line[1024];

  FILE *out_fp = NULL;

  // 如果是调试模式，打开输出文件

  out_fp = fopen("Minimake_cleared.mk", "w");
  if (out_fp == NULL) {
    LogError("Failed to create output file");
    fclose(fp);
    return -1;
  }

  // 逐行读取文件
  while (fgets(line, sizeof(line), fp) != NULL) {
    int len;

    // 去除注释
    char *comment = strchr(line, '#');
    if (comment != NULL) {
      *comment = '\0';
    }

    // 去除行尾空格和换行符
    len = strlen(line);
    while (len > 0 && isspace(line[len - 1])) {
      line[--len] = '\0';
    }

    // 跳过空行
    if (len == 0) {
      continue;
    }
    // 输出处理后的行
    // LogDebug("Processed line: %s", line);

    fprintf(out_fp, "%s\n", line);
  }
  return 0;
}

/**
 * @brief 解析Makefile文件
 *
 * @return int
 */
int MkParser(MkTarget_p *targets) {
  int targetsSize = 10;
  *targets = (MkTarget_p)malloc(sizeof(MkTarget_t) * targetsSize);
  int targetNum = 0;
  LogDebug("MkParser");
  // LogDebug("Parsing %s", arg);
  // 检查cwd下是否存在Makefile文件
  FILE *fp = fopen("Makefile", "r");
  if (fp == NULL) {
    LogError("Makefile not found");
    return -1;
  }
  char line[1024];

  int lineNum = 0;
  int commandsSize = 0;
  // 逐行读取文件
  while (fgets(line, sizeof(line), fp) != NULL) {
    lineNum++;
    int len;

    // 去除注释
    char *comment = strchr(line, '#');
    if (comment != NULL) {
      *comment = '\0';
    }

    // 去除行尾空格和换行符
    len = strlen(line);
    while (len > 0 && isspace(line[len - 1])) {
      line[--len] = '\0';
    }

    // 跳过空行
    if (len == 0) {
      continue;
    }

    // 进行静态语法检查
    // 如果行首是不是制表符，那么这一行是一个新的规则
    if (line[0] != '\t') {
      // LogDebug("Rule: %s", line);
      // 这一行是一个新的规则, 必须有冒号
      char *colon = strchr(line, ':');
      if (colon == NULL) {
        LogError("Line:%d: Missing colon in target definition", lineNum);
        exit(1);
      }
      targetNum++;
      if (targetNum > targetsSize) {
        targetsSize += 10;
        (*targets) =
            (MkTarget_p)realloc((*targets), sizeof(MkTarget_t) * targetsSize);
        if (*targets == NULL) {
          LogError("realloc failed");
          exit(1);
        }
      }
      // 初始化目标为NULL
      (*targets)[targetNum - 1].name = NULL;
      (*targets)[targetNum - 1].deps = NULL;
      (*targets)[targetNum - 1].commands = NULL;
      (*targets)[targetNum - 1].deps_p = NULL;
      (*targets)[targetNum - 1].depsSize = 0;
      (*targets)[targetNum - 1].commandsSize = 0;
      (*targets)[targetNum - 1].name = (char *)malloc(colon - line + 1);
      // 打印目标名称地址
      // LogDebug("Target %d: %p", targetNum - 1, (*targets)[targetNum -
      // 1].name); LogDebug("Target: %s", (*targets)[targetNum - 1].name);
      strncpy((*targets)[targetNum - 1].name, line, colon - line);
      // LogDebug("Target: %s", (*targets)[targetNum - 1].name);
      (*targets)[targetNum - 1].name[colon - line] = '\0';
      LogDebug("Target: %s", (*targets)[targetNum - 1].name);
      // 依赖目标
      // 跳过冒号后的空格
      // LogDebug("Colon: %s", colon);
      colon++;
      while (*colon != '\0' && isspace(*colon)) {
        colon++;
      }
      // LogDebug("Colon: %s", colon);
      char *space = colon;  // 冒号后的第一个字符
      // LogDebug("Space: %s", space);
      // depsSize的初始最大值
      int depsSize = 10;
      (*targets)[targetNum - 1].deps =
          (char **)malloc(sizeof(char *) * depsSize);
      (*targets)[targetNum - 1].depsSize = 0;
      while (*space != '\0') {
        char *next_space = strchr(space, ' ');
        if (next_space == NULL) {
          next_space = space + strlen(space);
        }
        if ((*targets)[targetNum - 1].depsSize >= depsSize) {
          depsSize += 10;
          (*targets)[targetNum - 1].deps = (char **)realloc(
              (*targets)[targetNum - 1].deps, sizeof(char *) * depsSize);
          if ((*targets)[targetNum - 1].deps == NULL) {
            LogError("realloc failed");
            exit(1);
          }
        }
        (*targets)[targetNum - 1].deps[(*targets)[targetNum - 1].depsSize] =
            (char *)malloc(next_space - space + 1);
        strncpy(
            (*targets)[targetNum - 1].deps[(*targets)[targetNum - 1].depsSize],
            space, next_space - space);
        (*targets)[targetNum - 1]
            .deps[(*targets)[targetNum - 1].depsSize][next_space - space] =
            '\0';
        (*targets)[targetNum - 1].depsSize++;
        if (*next_space == '\0') {
          break;
        }
        space = next_space + 1;
        while (*space != '\0' && isspace(*space)) {
          space++;
        }
      }
      // 初始化commandsSize
      commandsSize = 10;
      (*targets)[targetNum - 1].commands =
          (char **)malloc(sizeof(char *) * commandsSize);
      // MkDisplay(&(*targets)[targetNum - 1]);
    } else {
      // LogDebug("Command: %s", line);
      // 如果是命令行，那么前面必须有一个规则
      if (lineNum == 1) {
        LogError("Line1: Command found before rule");
        exit(1);
      }
      commandsSize++;
      if (commandsSize > (*targets)[targetNum - 1].commandsSize) {
        commandsSize += 10;
        (*targets)[targetNum - 1].commands = (char **)realloc(
            (*targets)[targetNum - 1].commands, sizeof(char *) * commandsSize);
        if ((*targets)[targetNum - 1].commands == NULL) {
          LogError("realloc failed");
          exit(1);
        }
      }
      (*targets)[targetNum - 1]
          .commands[(*targets)[targetNum - 1].commandsSize] =
          (char *)malloc(len);
      strcpy((*targets)[targetNum - 1]
                 .commands[(*targets)[targetNum - 1].commandsSize],
             line + 1);
      (*targets)[targetNum - 1]
          .commands[(*targets)[targetNum - 1].commandsSize][len - 1] = '\0';
      (*targets)[targetNum - 1].commandsSize++;
      // MkDisplay(&(*targets)[targetNum - 1]);
      // TODO: commands
    }
    // LogDebug("Processed line: %s", line);
    // MkDisplay(&(*targets)[targetNum - 1]);
  }

  return targetNum;
}
/**
 * @brief free MkTarget_t
 *
 */
int MkFree(MkTarget_p target) {
  LogDebug("MkFree");
  if (target == NULL) {
    LogError("target is NULL");
    return -1;
  }
  free(target->name);
  for (int i = 0; i < target->depsSize; i++) {
    free(target->deps[i]);
    // free(target->deps_p[i]);
  }
  free(target->deps);
  for (int i = 0; i < target->commandsSize; i++) {
    free(target->commands[i]);
  }
  free(target->commands);  // TODO: free commands
  target = NULL;
  return 0;
}

/**
 * @brief free MkTarget_t[]
 *
 */
int FreeMkTargets(MkTarget_p *targets, int targetNum) {
  LogDebug("FreeMkTargets");
  for (int i = 0; i < targetNum; i++) {
    if (logger_config.level == LOG_DEBUG) {
      MkDisplay((*targets) + i);
    }
    // LogDebug("Target %d: %p", i, (*targets)[i].name);
    if ((*targets)[i].name == NULL) {
      LogError("targets[%d] is NULL", i);
      break;
    }
    MkFree(((*targets) + i));
  }
  free((*targets));
  (*targets) = NULL;
  return 0;
}

/**
 * @brief display MkTarget_t
 *
 */
int MkDisplay(MkTarget_p target) {
  LogDebug("MkDisplay");
  LogInfo("Target: %s", target->name);
  LogInfo("DepSize: %d", target->depsSize);
  for (int i = 0; i < target->depsSize; i++) {
    LogInfo("Dep%d: %s", i + 1, target->deps[i]);
  }
  LogInfo("CommandSize: %d", target->commandsSize);
  for (int i = 0; i < target->commandsSize; i++) {
    LogInfo("Command%d: %s", i + 1,
            target->commands[i]);  // TODO: display commands
  }
  return 0;
}

/**
 * @brief Check MKTarget_p*
 * @param targets
 * @param targetNum
 * @return int
 * @note Check if the targets' deps are valid
 */
int MkDepCheck(MkTarget_p *targets, int targetNum) {
  LogDebug("MkCheck");
  int ret = 0;
  for (int i = 0; i < targetNum; i++) {
    for (int j = 0; j < (*targets)[i].depsSize; j++) {
      int found = 0;
      for (int k = 0; k < targetNum; k++) {
        if (strcmp((*targets)[i].deps[j], (*targets)[k].name) == 0) {
          found = 1;
          break;
        }
      }
      if (found == 0) {
        // 检查依赖目标文件是否存在
        FILE *fp = fopen((*targets)[i].deps[j], "r");
        if (fp == NULL) {
          LogError("Invalid dependency '%s'", (*targets)[i].deps[j]);
          ret = -1;
        }
      }
    }
  }
  return ret;
}

/**
 * @brief Check MKTarget_p*
 * @param targets
 * @param targetNum
 * @return int
 * @note Check if the targets' target are repeated
 */
int MkTargetCheck(MkTarget_p *targets, int targetNum) {
  LogDebug("MkCheck");
  int ret = 0;
  for (int i = 0; i < targetNum; i++) {
    for (int j = i + 1; j < targetNum; j++) {
      if (strcmp((*targets)[i].name, (*targets)[j].name) == 0) {
        LogError("Duplicate target definition '%s'", (*targets)[i].name);
        ret = -1;
      }
    }
  }
  return ret;
}