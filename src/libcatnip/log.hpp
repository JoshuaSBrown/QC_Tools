// Taken from
// https://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming

#ifndef _CATNIP_LOG_HPP_
#define _CATNIP_LOG_HPP_

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#include <iostream>

namespace catnip {

#define LOG(msg, log_level) \
  (Log(__FILE__, __LINE__, LogData<None>() << msg, log_level))

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
#ifdef __ICC
#define NOINLINE_ATTRIBUTE __attribute__((noinline))
#else
#define NOINLINE_ATTRIBUTE
#endif  // __ICC
#endif  // NOINLINE_ATTRIBUTE

struct None {};

template <typename List>
struct LogData {
  List list;
};

template <typename List>
void Log(const char* file, int line, LogData<List>&& data,
         int log_level) NOINLINE_ATTRIBUTE {
  if (log_level <= LOG_LEVEL) {
    std::cout << file << ":" << line << ": ";
    output(std::cout, std::move(data.list));
    std::cout << std::endl;
  }
}

template <typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(
    LogData<Begin>&& begin, Value&& value) noexcept {
  return {{std::forward<Begin>(begin.list), std::forward<Value>(value)}};
}

template <typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(
    LogData<Begin>&& begin, const char (&value)[n]) noexcept {
  return {{std::forward<Begin>(begin.list), value}};
}

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template <typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(
    LogData<Begin>&& begin, PfnManipulator value) noexcept {
  return {{std::forward<Begin>(begin.list), value}};
}

template <typename Begin, typename Last>
void output(std::ostream& os, std::pair<Begin, Last>&& data) {
  output(os, std::move(data.first));
  os << data.second;
}

inline void output(std::ostream& os, None) { os << ""; }

}  // namespace catnip
#endif  // _CATNIP_LOG_HPP_
