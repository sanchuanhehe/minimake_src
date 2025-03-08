#include "loger.h"
#include <stdio.h>



/**
 * @brief 解析Makefile文件
 * 
 * @param arg Makefile文件路径
 * @return int 
 */
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
    char line[1024];
    int verbose_mode = logger_config.level == LOG_DEBUG;
    FILE *out_fp = NULL;
    
    // 如果是调试模式，打开输出文件
    if (verbose_mode) {
        out_fp = fopen("Minimake_cleared.mk", "w");
        if (out_fp == NULL) {
            log_error("Failed to create output file");
            fclose(fp);
            return -1;
        }
    }

    int line_num = 0;

    // 逐行读取文件
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;
        char *p = line;
        int len;
        
        // 去除注释
        char *comment = strchr(line, '#');
        if (comment != NULL) {
            *comment = '\0';
        }
        
        // 去除行尾空格和换行符
        len = strlen(line);
        while (len > 0 && isspace(line[len-1])) {
            line[--len] = '\0';
        }
        
        // 跳过空行
        if (len == 0) {
            continue;
        }

        // 进行静态语法检查
        // 如果行首是不是空格或者制表符，那么这一行是一个新的规则
        if (!isspace(*p)) {
            log_debug("Rule: %s", line);
            // 这一行是一个新的规则, 必须有冒号
            char *colon = strchr(line, ':');
            if (colon == NULL) {
                log_error("Line:%d: Missing colon in target definition", line_num);
                exit(1);
            }
        } else {
            log_debug("Command: %s", line);
            // 如果是命令行，那么前面必须有一个规则
            if (line_num == 1) {
                log_error("Line1: Command found before rule");
                exit(1);
            }
            // 如果行首不是4个空格,报错
            if (line[0] != ' ' || line[1] != ' ' || line[2] != ' ' || line[3] != ' ') {
                log_error("Line:%d: Command must be indented with 4 spaces, not a Tab", line_num);
                exit(1);
            }
        }
        

        // 输出处理后的行
        log_debug("Processed line: %s", line);
        if (verbose_mode && out_fp != NULL) {
            fprintf(out_fp, "%s\n", line);
        }
    }

    if (out_fp != NULL) {
        fclose(out_fp);
    }
    fclose(fp);

    return 0;
}