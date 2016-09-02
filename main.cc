#include <iostream>
#include <vector>

#include <unistd.h>

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
    logger.log("in of scope {}", "hello");
  }

  logger.log("out of scope {}", "hello");

  TimerTree timer_tree;

  timer_tree.PushTick("level 1 - hello");
  for (int i = 0; i < 10000000; ++i) ;

  timer_tree.PushTick("level 2 - red");
  for (int i = 0; i < 10000000; ++i) ;
  timer_tree.PopTick("level 2 - red");

  timer_tree.PopTick("level 1 - hello");

  timer_tree.PushTick("level 1 - world");
  for (int i = 0; i < 10000000; ++i) ;
  timer_tree.PopTick("level 1 - world");

  timer_tree.PushTick("level 1 - world");
  for (int i = 0; i < 10000000; ++i) ;
  timer_tree.PopTick("level 1 - world");


  cout << "report" << endl;
  cout << timer_tree.Report() << endl;

  TickOnce tick;
  for (int i = 0; i < 10000000; ++i) ;
  cout << tick.Tick().count() << endl;
  cout << tick.TickString() << endl;


  tick.TickMs();
  sleep(1);
  cout << tick.TickMs() << "ms" << endl;

  return 0;
}

