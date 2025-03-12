#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "loger.h"
#include "mk_parser.h"

/**
 * @brief try to run the target
 *
 */
int MkRun(MkTarget_p target) {
  LogDebug("MkRun");
  if (target == NULL) {
    LogError("target is NULL");
    return -1;
  }
  if (target->commandsSize == 0) {
    LogError("No command found");
    return -1;
  }
  for (int i = 0; i < target->commandsSize; i++) {
    LogInfo("Command: %s", target->commands[i]);
    int ret = fputs(target->commands[i], stdout);
    if (ret == EOF) {
      LogError("fputs failed");
      return -1;
    }
    putchar('\n');
    ret = system(target->commands[i]);
    if (ret != 0) {
      LogError("Command failed: %s", target->commands[i]);
      LogError("ret: %d", ret);
      return -1;
    }
  }
  return 0;
}

/**
 * @brief run all targets with order sorted by @ref MkGraphTopologicalSort
 *
 */
int MkRunAll(MkTarget_p targets, int targetNum, igraph_vector_int_t *order) {
  LogDebug("MkRunAll");
  targetNum = igraph_vector_int_size(order);
  for (int i = 0; i < targetNum; i++) {
    int ret = MkRun(&targets[VECTOR(*order)[i]]);
    if (ret != 0) {
      LogError("MkRun failed");
      return -1;
    }
  }
  return 0;
}

/**
 * @brief run from target
 *
 * @param targets
 * @param targetNum
 * @param order
 * @param targetName  target name for run start
 */
int MkRunFrom(MkTarget_p targets, int targetNum, char *targetName,
              igraph_t *graph) {
  LogDebug("MkRunFrom");
  int start = -1;
  for (int i = 0; i < targetNum; i++) {
    if (strcmp(targets[i].name, targetName) == 0) {
      start = i;
      LogInfo("target: %s is found at %d", targetName);
      break;
    }
  }
  if (start == -1) {
    LogError("Target not found: %s", targetName);
    return -1;
  }
  // 找到起始点处的顶点id,并从此处开始获得构建子图
  igraph_vector_int_t subcomponent;
  igraph_vector_int_init(&subcomponent, 0);
  igraph_subcomponent(graph, &subcomponent, start, IGRAPH_OUT);

  // 创建子图
  igraph_t subgraph;
  igraph_induced_subgraph(graph, &subgraph, igraph_vss_vector(&subcomponent),
                          IGRAPH_SUBGRAPH_CREATE_FROM_SCRATCH);

  // 打印子图中的顶点
  LogDebug("Subgraph:");
  MkGraphDisplay(&subgraph, &targets, targetNum);
  igraph_vector_int_t *order = NULL;
  order = (igraph_vector_int_t *)malloc(sizeof(igraph_vector_int_t) * targetNum);
  igraph_vector_int_init(order, targetNum);
  int ret = MkGraphTopologicalSort(&subgraph, order);
  if (ret != 0) {
    LogError("MkGraphTopologicalSort failed");
    return -1;
  }
  ret = MkRunAll(targets, targetNum, order);
  if (ret != 0) {
    LogError("MkRunAll failed");
    return -1;
  }

  // 释放资源
  igraph_vector_int_destroy(&subcomponent);
  igraph_destroy(&subgraph);

  return 0;
}