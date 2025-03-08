#include "loger.h"
#include <stdio.h>
int mk_parser(const char *arg)
{
    log_debug("Parsing %s\n", arg);
    // 检查cwd下是否存在Makefile文件
    FILE *fp = fopen("Makefile", "r");
    if (fp == NULL)
    {
        log_error("Makefile not found\n");
        return -1;
    }
    fclose(fp);

    return 0;
}