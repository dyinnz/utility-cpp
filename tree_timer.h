/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-06-06
 ******************************************************************************/

#pragma once

#include <unordered_map>
#include <string>
#include <chrono>
#include <memory>
#include <stack>
#include <sstream>

class TickOnce {
  public:

  private:

};

class TimerNode {
  public:
    TimerNode();

    void Restart();
    void Resume();
    void Pause();

    std::chrono::system_clock::duration Span();

    void StartSubTick(const std::string &label);
    void EndSubTick();
    
    std::shared_ptr<TimerNode> GetChild(const std::string &label);

    std::string Report(int deep = 0);

  private:
    void Report(std::ostringstream &ss, int deep, const std::string &label);

  private:
    // children
    std::unordered_map<std::string, std::shared_ptr<TimerNode>> _chidren;
    std::shared_ptr<TimerNode> _current_child;

    // self
    std::chrono::system_clock::time_point _last;
    std::chrono::system_clock::duration _acculumate {0};
    bool _is_pause {false};
};


class TimerTree {
  public:
    TimerTree(const std::string &label = "ROOT") :
      _root_label(label), _root(new TimerNode) {
        _stack.push(_root);
      }

    void PushTick(const std::string &label);
    void PopTick();
    void ReplaceTick(const std::string &label);

    TimerNode& CurrentNode() { return *_stack.top(); }

    std::string Report();

  private:
    std::string _root_label;
    std::shared_ptr<TimerNode> _root;
    std::stack<std::shared_ptr<TimerNode>> _stack;
};

