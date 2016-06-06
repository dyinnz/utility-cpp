#include <iostream>

#include "simplelogger.h"
#include "utility.h"

using namespace std;
using namespace simple_logger;

int main() {
  BaseLogger logger(kDebug);
  // BaseLogger logger;

  logger.debug("hello world");
  logger.log("hello world");
  logger.notice("hello world");
  logger.error("hello world");

  logger.log("hello {} world {}", 123, 1.23);

  /*
  {
    ScopeGuard {
      cout << "hello world" << endl;
    };
  }
  */

  logger.log("out of scope {}", "helo");

  {
    ScopeGuarder sg = [&]() {
      cout << "hello world" << endl;
    };
  }


  return 0;
}
