#ifndef MK_RUNNER_H
#define MK_RUNNER_H

#include "loger.h"
#include "mk_parser.h"

/**
 * @brief try to run the target
 *
 */
int MkRun(MkTarget_p target);

/**
 * @brief run all targets with order sorted by @ref MkGraphTopologicalSort
 * @note for debug
 */
int MkRunAll(MkTarget_p targets, int targetNum, igraph_vector_int_t *order);

/**
 * @brief run from target
 *
 * @param targets
 * @param targetNum
 * @param targetName  target name for run start
 */
int MkRunFrom(MkTarget_p targets, int targetNum, char *targetName,igraph_t *graph);
#endif