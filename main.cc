#include <iostream>

#include "unistd.h"
#include "simplelogger.h"
#include "utility.h"
#include "tree_timer.h"

using namespace std;
using namespace std::chrono;
using namespace simple_logger;

int main() {
  BaseLogger logger(kDebug);
  // BaseLogger logger;

  logger.debug("hello world");
  logger.log("hello world");
  logger.notice("hello world");
  logger.error("hello world");

  logger.log("hello {} world {}", 123, 1.23);

  {
    ScopeGuard {
      cout << "hello world" << endl;
    };
  }

  logger.log("out of scope {}", "hello");

  TimerTree timer_tree;

  timer_tree.PushTick("level 1 - hello");
  for (int i = 0; i < 10000000; ++i) ;

  timer_tree.PushTick("level 2 - red");
  for (int i = 0; i < 10000000; ++i) ;
  timer_tree.PopTick();

  timer_tree.PopTick();

  timer_tree.PushTick("level 1 - world");
  for (int i = 0; i < 10000000; ++i) ;
  timer_tree.PopTick();

  timer_tree.PushTick("level 1 - world");
  for (int i = 0; i < 10000000; ++i) ;
  timer_tree.PopTick();

  cout << "report" << endl;
  cout << timer_tree.Report() << endl;

  return 0;
}

