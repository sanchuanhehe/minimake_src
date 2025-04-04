#ifndef MK_PARSER_H
#define MK_PARSER_H
#include "igraph/igraph.h"
// Forward declaration
typedef struct MkTarget *MkTarget_p;
typedef struct MkTarget MkTarget_t;
/**
 * @brief make 目标结构体
 */
struct MkTarget {
  char *name;          //< 目标名称
  // MkTarget_p *deps_p;  //< 依赖目标指针
  char **deps;         //< 依赖目标
  char **commands;     //< 执行命令
  int depsSize;        //< 依赖目标数量
  int commandsSize;    //< 执行命令数量
  igraph_integer_t vid;  //< 顶点id
  igraph_vector_int_t *vdeps;  //< 依赖顶点
};
/**
 * @brief 清理makefile文件
 *
 * @return int
 */
int MkClean();
/**
 * @brief 解析Makefile文件
 *
 * @param targets 解析结果
 * @param targetNum 解析结果数量
 * @return int targetNum
 * @note 解析成功返回解析结果数量，解析失败返回-1
 * @note 解析结果需要在外部释放,targets需要free
 */
int MkParser(MkTarget_p *targets);

/**
 * @brief free MkTarget_p
 *
 */
int MkFree(MkTarget_p target);

/**
 * @brief free MkTarget_t[]
 *
 */
int FreeMkTargets(MkTarget_p *targets, int targetNum);

/**
 * @brief display MkTarget_t
 *
 */
int MkDisplay(MkTarget_p target);

/**
 * @brief Check MKTarget_p*
 * @param targets
 * @param targetNum
 * @return int
 * @note Check if the targets' deps are valid
 */

int MkDepCheck(MkTarget_p *targets, int targetNum);

/**
 * @brief Check MKTarget_p*
 * @param targets
 * @param targetNum
 * @return int
 * @note Check if the targets' target are repeated
 */
int MkTargetCheck(MkTarget_p *targets, int targetNum);

/**
 * @brief init MkGraph
 * 
 * @param graph
 * @param targets
 * @param targetNum
 * @return int
 * @note init graph with targets and fiind every target's deps (vdeps)
 */
int MkGraphInit(igraph_t *graph, MkTarget_p *targets, int targetNum);

/**
 * @brief Display MkGraph
 * 
 * @param graph
 * @return int
 * @note Display graph
 */
int MkGraphDisplay(igraph_t *graph, MkTarget_p *targets, int targetNum);

/**
 * @brief topological sort
 * 
 * @param graph
 * @param order
 * @return int
 * @note topological sort
 */
int MkGraphTopologicalSort(igraph_t *graph, igraph_vector_int_t *order);
#endif