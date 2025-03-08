#ifndef MK_PARSER_H
#define MK_PARSER_H

/**
 * @brief make 目标结构体
 */
struct mk_target
{
    char *name;       //< 目标名称
    char **deps;   //< 依赖目标
    char **commands; //< 执行命令
};

/**
 * @brief 解析Makefile文件
 * 
 * @param arg Makefile文件路径
 * @return int 
 */
int mk_parser(const char *arg);

#endif