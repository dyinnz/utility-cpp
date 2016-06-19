/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-06-06
 ******************************************************************************/

#include <cassert>
#include <iomanip>
#include "tree_timer.h"

using namespace std::chrono;
using std::endl;
using std::string;
using std::ostringstream;
using std::ostream;
using std::shared_ptr;
using std::make_shared;

/*----------------------------------------------------------------------------*/

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

    _current_child = make_shared<TimerNode>(label);
    _chidren[label] = _current_child;

  } else {
    _current_child = iter->second;
    _current_child->Resume();
  }
}


void TimerNode::EndSubTick(const std::string &label) {
  assert(_current_child);

  if (_current_child->label() != label) {
    assert(false);
  }

  _current_child->Pause();
  _current_child.reset();
}


string TimerNode::Report(int deep) {
  ostringstream ss;
  print_indent(ss, deep);
  ss << "+ " << _label << ": " 
    << std::setw(12) << std::setprecision(4) << std::fixed
    << duration_to_ms(Span()) << "ms    100%" << endl;
  RecursiveReport(ss, 0);
  return ss.str();
}


void TimerNode::RecursiveReport(ostream &out, int deep) {
  float total = duration_to_ms(Span());

  float unaccounted {total};

  for (auto &p : _chidren) {
    float child_ms = duration_to_ms(p.second->Span());
    unaccounted -= child_ms;

    print_indent(out, deep+1);
    out << "+ " << p.first << ": "
      << std::setw(10) << std::setprecision(4) << std::fixed
      << child_ms << "ms  " 
      << std::setw(10) << std::setprecision(2) << std::fixed
      << child_ms / total * 100.0f << "%" << endl;

    p.second->RecursiveReport(out, deep+1);
  }

  if (!_chidren.empty() && unaccounted / total > 0.00001f) {
    print_indent(out, deep+1);
    out << "+ Unaccounted: " 
      << std::setw(10) << std::setprecision(4) << std::fixed 
      << unaccounted << "ms  "
      << std::setw(10) << std::setprecision(2) << std::fixed
      << unaccounted / total * 100.0f << "%" << endl;
  }
}


/*----------------------------------------------------------------------------*/


TimerTree::TimerTree(const std::string &label)
: _root_label(label), _root(new TimerNode(label)) {

  _stack.push(_root);
}


void TimerTree::PushTick(const std::string &label) {
  _stack.top()->StartSubTick(label);
  _stack.push( _stack.top()->GetChild(label) );
}


void TimerTree::PopTick(const std::string &label) {
  if (label != _stack.top()->label()) {
    assert(false);
  }

  _stack.pop();
  _stack.top()->EndSubTick(label);
}


void TimerTree::ReplaceTick(const std::string &old_label,
                            const std::string &new_label) {
  PopTick(old_label);
  PushTick(new_label);
}


string TimerTree::Report() {
  return _root->Report();
}

