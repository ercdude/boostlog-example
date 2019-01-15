#ifndef LOG_HPP
#define LOG_HPP

#include <sstream>

#define LOG(x) Log::Logger(x)

enum LogLvl {
             trace,
             debug,
             info,
             warning,
             error,
             fatal
};

namespace Log {
  class Logger
  {
  public:
    Logger(LogLvl lvl);

    ~Logger();

    static void init();

    template<typename T>
    Logger& operator << (const T &message) {
      _ss << message;
      return *this;
    }

  private:
    LogLvl _lvl;

    std::stringstream _ss;
  };
}

#endif
