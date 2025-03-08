#include "loger.h"
#include <stdio.h>
int mk_parser(const char *arg)
{
    log_debug("mk_parser");
    log_debug("Parsing %s", arg);
    // 检查cwd下是否存在Makefile文件
    FILE *fp = fopen("Makefile", "r");
    if (fp == NULL)
    {
        log_error("Makefile not found");
        return -1;
    }
    fclose(fp);

    return 0;
}