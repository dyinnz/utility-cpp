/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-04-05
 ******************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include <mutex>

using namespace std;

class MTLogger {
  public:
    enum Level { kDebug = 0, kLog = 1, kNotice = 2, kError = 3, kSilence };
    // static constexpr int kLevelNumber = 3;

    MTLogger(Level log_level = kError) : _log_level(log_level) {
    }

    void set_log_level(Level log_level) {
      _m.lock();
      _log_level = log_level;
      _m.unlock();
    }

    Level log_level() {
      return _log_level;
    }

    template<typename ...A> void debug(A... args) {
      if (_log_level <= kDebug) {
        print_preamble("[Debug]  ", args...);
      }
    }
    template<typename ...A> void log(A... args) {
      if (_log_level <= kLog) {
        print_preamble("[Log]    ", args...);
      }
    }
    template<typename ...A> void notice(A... args) {
      if (_log_level <= kNotice) {
        print_preamble("[Notice] ", args...);
      }
    }
    template<typename ...A> void error(A... args) {
      if (_log_level <= kError) {
        print_preamble("[Error]  ", args...);
      }
    }

  private:
    template<typename ...A> void print_preamble(const char *tag, A... args) {
      _m.lock();
      cout << tag;
      do_print(args...);
      _m.unlock();
    }

    void do_print(const char *format) {
      cout << format << endl;
    }

    template<typename T, typename ...A> void
      do_print(const char *format, const T &v, A... args) {
        const char *s = format;
        while (s && *s) {
          switch(*s) {
            case '{':
              if ('}' == *(s+1)) {
                cout << v;
                return do_print(s+2, args...);

              } else {
                cout << *s;
                s+= 1;
              }
              break;

            case '\\':
              if ('\0' != *(s+1)) {
                cout << *(s+1);
                s += 2;
              }
              break;

            default:
              cout << *s;
              s += 1;
              break;
          }
        }
        cout << endl;
        assert(false);
    }

  private:
    std::mutex _m;
    Level _log_level { kError };

  private:
    MTLogger(const MTLogger&) = delete;
    MTLogger(MTLogger&&) = delete;
    MTLogger operator=(const MTLogger&) = delete;
    MTLogger operator=(MTLogger&&) = delete;
};

