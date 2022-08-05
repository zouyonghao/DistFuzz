#if !defined(__FUZZ_LOG_HEADER__)
#define __FUZZ_LOG_HEADER__

#include <boost/log/core.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#ifndef NDEBUG
#define DEBUG_LOG(X)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        std::cout << X;                                                                                                \
    } while (false)
#else
#define DEBUG_LOG(X)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
    } while (false)
#endif

#ifndef NDEBUG
#define IS_DEBUG true
#else
#define IS_DEBUG false
#endif

#define LOG_ERROR BOOST_LOG_TRIVIAL(error)

#define LOG_INFO BOOST_LOG_TRIVIAL(info)

static void log_init(std::string log_file_name)
{
    namespace logging = boost::log;
    namespace keywords = boost::log::keywords;
    logging::add_file_log(keywords::file_name = log_file_name, keywords::target_file_name = log_file_name,
                          keywords::auto_flush = true);
}

#endif // __FUZZ_LOG_HEADER__