/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-06-04
 ******************************************************************************/

#pragma once

#include <cassert>
#include <sstream>
#include <string>

#define DEBUG

namespace simplelogger {

#define func_debug(__logger, __format, ...) do { \
  __logger.debug("{}() " __format, __func__, ##__VA_ARGS__); \
} while (false);

#define func_log(__logger, __format, ...) do { \
  __logger.log("{}() " __format, __func__, ##__VA_ARGS__); \
} while (false);

#define func_notice(__logger, __format, ...) do { \
  __logger.notice("{}() " __format, __func__, ##__VA_ARGS__); \
} while (false);

#define func_error(__logger, __format, ...) do { \
  __logger.error("{}() " __format, __func__, ##__VA_ARGS__); \
} while (false);

/**
 * log level
 */
enum Level {
  kDebug     = 0,
  kLog       = 1,
  kNotice    = 2,
  kError     = 3,
  kSilence,
};

constexpr int kMaxLevel {5};

/**
 * auxiliary function
 */
inline void parse_format(std::ostringstream &ss, const char *format) { ss << format; }

template<typename T, typename ...A>
void parse_format(std::ostringstream &ss, const char *format, const T &v, A... args);


/*------------------------------------------------------------------*/

/**
 * logger for single thread
 */
class Logger {
public:
  Logger(Level level = kLog) : _log_level(level) {
    for (int l = 0; l < kMaxLevel; ++l) {
      _fp[l] = stdout;
    }
  }

  void set_log_level(Level level) { _log_level = level; }
  Level log_level() { return _log_level; };

  void set_level_file(Level level, FILE *fp) { _fp[level] = fp; }

  void flush(Level level) { fflush(_fp[level]); }
  void flush() {
    for (int l = 0; l < kMaxLevel; ++l) {
      flush(static_cast<Level>(l));
    }
  }

  template<typename ...A> void debug  (A... args) {
#ifdef DEBUG
    print_wrapper (kDebug, args...);
#endif
  }

  template<typename ...A> void log    (A... args) {
    print_wrapper (kLog, args...);
  }
  template<typename ...A> void notice (A... args) {
    print_wrapper (kNotice, args...);
  }
  template<typename ...A> void error  (A... args) {
    print_wrapper (kError, args...);
  }

private:
  template<typename ...A> void print_wrapper(Level level, A... args);

  Level _log_level { kError };
  FILE *_fp[kMaxLevel];

  const char *kTag[kMaxLevel] {
      "[DEBUG]  ",
      "[LOG]    ",
      "[NOTICE] ",
      "[ERROR]  ",
      ""
  };
};


/*------------------------------------------------------------------*/
// implement

template<typename T, typename ...A>
void parse_format(std::ostringstream &ss,
                  const char        *format,
                  const T           &v,
                  A...              args) {

  assert(format);
  const char *p { format };

  while (*p) {
    switch (*p) {

      case '{':
        if ('}' == *(p+1)) {
          ss << v;
          parse_format(ss, p+2, args...);
          return;

        } else {
          ss << *p;
          p += 1;
        }
        break;

      case '%':
        if ('\0' != *(p+1)) {
          ss << *(p+1);
          p += 2;
        }
        break;

      default:
        ss << *p;
        p += 1;
        break;
    }
  }
}

/*----------------------------------------------------------------------------*/



template<typename ...A>
void Logger::print_wrapper(Level level, A... args) {

  if (log_level() <= level) {
    std::ostringstream ss;
    ss << kTag[level];
    parse_format(ss, args...);
    ss << "\n";
    fwrite(ss.str().c_str(), 1, ss.str().size(), _fp[level]);
  }
}

} // end of namespace simple_logger
