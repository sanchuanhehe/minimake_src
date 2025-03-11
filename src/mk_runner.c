#include "mk_parser.h"
#include "loger.h"
#include "unistd.h"
#include "stdlib.h"

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
    int ret = system(target->commands[i]);
    if (ret != 0) {
      LogError("Command failed: %s", target->commands[i]);
      return -1;
    }
  }
  return 0;
}