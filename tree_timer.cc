/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-06-06
 ******************************************************************************/

#include <cassert>
#include <iostream>
#include "tree_timer.h"

using namespace std::chrono;
using std::endl;
using std::string;
using std::ostringstream;
using std::shared_ptr;
using std::make_shared;

/*----------------------------------------------------------------------------*/

TickOnce::TickOnce() : _last(system_clock::now()) {
  // empty
}


system_clock::duration TickOnce::Tick() {
  auto ret = system_clock::now() - _last;
  _last = system_clock::now();
  return ret;
}

string TickOnce::TickString() {
  ostringstream ss;
  ss << Tick().count() / 1000000.0 << "ms" << std::ends;
  return move(ss.str());
}

/*----------------------------------------------------------------------------*/

TimerNode::TimerNode() : _last(system_clock::now()) {
  // empty
}


void TimerNode::Restart() {
  _chidren.clear();
  _current_child = nullptr;

  _acculumate.zero();
  _is_pause = false;
  _last = system_clock::now();
}


void TimerNode::Resume() {
  if (_is_pause) {
    if (_current_child) {
      _current_child->Resume();
    }

    _is_pause = false;
    _last = system_clock::now();
  }
}


void TimerNode::Pause() {
  if (!_is_pause) {
    if (_current_child) {
      _current_child->Pause();
    }

    _is_pause = true;
    _acculumate += system_clock::now() - _last;
  }
}


system_clock::duration TimerNode::Span() {
  if (_is_pause) {
    return _acculumate;
  } else {
    return _acculumate + system_clock::now() - _last;
  }
}


void TimerNode::StartSubTick(const std::string &label) {
  auto iter = _chidren.find(label);
  if (iter == _chidren.end()) {

    _current_child = make_shared<TimerNode>();
    _chidren[label] = _current_child;

  } else {
    _current_child = iter->second;
    _current_child->Resume();
  }
}


void TimerNode::EndSubTick() {
  assert(_current_child);
  _current_child->Pause();
  _current_child.reset();
}


shared_ptr<TimerNode> TimerNode::GetChild(const string &label) {
  return _chidren[label];
}


string TimerNode::Report(int deep) {
  ostringstream ss;
  Report(ss, 0, "ROOT");
  return ss.str();
}


void TimerNode::Report(ostringstream &ss, int deep, const string &label) {
  Pause();

  // report self
  for (int i = 0; i < deep*2; ++i) {
    ss << ' ';
  }
  ss << "+ " << label << " : ";
  ss << Span().count() << endl;

  system_clock::duration sum {0};

  // report children
  for (auto &p : _chidren) {
    p.second->Report(ss, deep + 1, p.first);
    sum += p.second->Span();
  }

  // report unaccounted time
  auto unaccounted = Span() - sum;
  if (!_chidren.empty() && float(unaccounted.count() / float(Span().count())) > 0.00001) {
    for (int i = 0; i < deep*2+2; ++i) {
      ss << ' ';
    }
    ss << "+ Unaccounted : " << (Span() - sum).count() << endl;
  }

  Resume();
}

/*----------------------------------------------------------------------------*/

TimerTree::TimerTree(const std::string &label) 
: _root_label(label), _root(new TimerNode) {

  _stack.push(_root);
}

void TimerTree::PushTick(const std::string &label) {
  _stack.top()->StartSubTick(label);
  _stack.push( _stack.top()->GetChild(label) );
}


void TimerTree::PopTick() {
  _stack.pop();
  _stack.top()->EndSubTick();
}


void TimerTree::ReplaceTick(const std::string &label) {
  PopTick();
  PushTick(label);
}


string TimerTree::Report() {
  return _root->Report();
}

