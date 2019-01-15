#include "log.hpp"

#include <boost/log/attributes/clock.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/keywords/auto_flush.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/rotation_size.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/keywords/max_size.hpp>
#include <boost/log/keywords/min_free_space.hpp>
#include <boost/log/keywords/target.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

#include <map>
#include <utility>

namespace BLog = boost::log;
namespace expr = BLog::expressions;
namespace attr = BLog::attributes;
namespace keywords = BLog::keywords;
namespace sinks = BLog::sinks;

#define LOG_TRIVIAL(lvl)                                                \
  BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),    \
                               (::boost::log::keywords::severity = lvl))

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", attr::current_thread_id::value_type)

namespace {
  const std::map<LogLvl, BLog::trivial::severity_level> _severityMapper = {
    { LogLvl::trace, BLog::trivial::severity_level::trace },
    { LogLvl::debug, BLog::trivial::severity_level::debug },
    { LogLvl::info, BLog::trivial::severity_level::info },
    { LogLvl::warning, BLog::trivial::severity_level::warning },
    { LogLvl::error, BLog::trivial::severity_level::error },
    { LogLvl::fatal, BLog::trivial::severity_level::fatal }};

  void log_formatter(BLog::record_view const& rec, BLog::formatting_ostream& strm)
  {
    // Add timestamp
    //strm << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp") << "]"
    strm << "[" << rec[timestamp] << "]"
         << "[" << rec[thread_id] << "]"
         << "<" << rec[BLog::trivial::severity] << "> "
         << rec[expr::smessage];
  }
} 

namespace Log {
  Logger::Logger(LogLvl lvl) : _lvl(lvl) {}

  Logger::~Logger() {
    std::map<LogLvl, BLog::trivial::severity_level>::const_iterator pos = _severityMapper.find(_lvl);
    if (pos != _severityMapper.end()) {
      auto lvl = pos->second;
      LOG_TRIVIAL(lvl) << _ss.str();
    }
  }

  void Logger::init()
  {
    // Setup common attributes, without it timestamp and thread_id won't work.
    BLog::add_common_attributes();

    // Setup sink backend
    typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
    boost::shared_ptr< file_sink > sink(new file_sink(
                                                      keywords::file_name = "4d-%m%d%Y_%H%M.log",
                                                      keywords::rotation_size = 5 * 1024 * 1024,
                                                      keywords::open_mode = std::ios_base::app)
                                        );
    // Setup formatter
    sink->set_formatter(&log_formatter);

    BLog::core::get()->add_sink(sink);
  }
}
