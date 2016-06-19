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
#include <ostream>
#include <sstream>


class TickOnce {
  public:
    TickOnce() : _last(std::chrono::system_clock::now()) {}

    std::chrono::system_clock::duration Tick() {
      auto ret = std::chrono::system_clock::now() - _last;
      _last = std::chrono::system_clock::now();
      return ret;
    }

    float operator() () {
      return Tick().count() / 1000000.0f;
    }

    float TickMs() {
      return Tick().count() / 1000000.0f;
    }

    std::string TickString() {
      std::ostringstream ss;
      ss << TickMs() << "ms" << std::ends;
      return std::move(ss.str());
    }

  private:
    std::chrono::system_clock::time_point _last;
};


class TimerNode {
  public:
    TimerNode(const std::string label)
      : _last(std::chrono::system_clock::now()), _label(label) {}

    const std::string& label() { return _label; }

    void Restart();
    void Resume();
    void Pause();

    std::chrono::system_clock::duration Span();

    void StartSubTick(const std::string &label);
    void EndSubTick(const std::string &label);

    std::shared_ptr<TimerNode> GetChild(const std::string &label) {
      return _chidren[label];
    }

    std::string Report(int deep = 0);

  private:
    void RecursiveReport(std::ostream &out, int deep);

  private:
    // children
    std::unordered_map<std::string, std::shared_ptr<TimerNode>> _chidren;
    std::shared_ptr<TimerNode> _current_child;

    // self
    std::chrono::system_clock::time_point _last;
    std::chrono::system_clock::duration _acculumate {0};
    std::string _label;
    bool _is_pause {false};
};


class TimerTree {
  public:
    TimerTree(const std::string &label = "ROOT");

    void PushTick(const std::string &label);
    void PopTick(const std::string &label);
    void ReplaceTick(const std::string &old_label, const std::string &new_label);

    TimerNode& CurrentNode() { return *_stack.top(); }

    std::string Report();

  private:
    std::string _root_label;
    std::shared_ptr<TimerNode> _root;
    std::stack<std::shared_ptr<TimerNode>> _stack;
};


inline float duration_to_ms(std::chrono::system_clock::duration d) {
  return d.count() / 1000000.0f;
}

inline void print_indent(std::ostream &out, int deep) {
  while (deep--) out << "    ";
}
