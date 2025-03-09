#ifndef MK_PARSER_H
#define MK_PARSER_H

// Forward declaration
typedef struct MkTarget *MkTarget_p;

/**
 * @brief make 目标结构体
 */
typedef struct MkTarget
{
    char *name;       //< 目标名称
    MkTarget_p *deps_p; //< 依赖目标指针
    char **deps;      //< 依赖目标
    int depsSize;     //< 依赖目标数量
    char **commands; //< 执行命令
    int commandsSize; //< 执行命令数量
} MkTarget_t;

/**
 * @brief 解析Makefile文件
 * 
 * @param arg Makefile文件路径
 * @param targets 解析结果
 * @param targetNum 解析结果数量
 * @return int targetNum
 * @note 解析成功返回解析结果数量，解析失败返回-1
 * @note 解析结果需要在外部释放,targets需要free
 */
int MkParser(const char *arg, MkTarget_p targets);

/**
 * @brief free MkTarget_p
 * 
 */
int MkFree(MkTarget_p target);

/**
 * @brief free MkTarget_t[]
 * 
 */
int FreeMkTargets(MkTarget_p targets, int targetNum);
#endif