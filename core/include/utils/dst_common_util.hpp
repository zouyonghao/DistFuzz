#ifndef __DST_COMMON_UTILS_H__
#define __DST_COMMON_UTILS_H__

#include <sstream>
#include <string>

#include <boost/process.hpp>

static std::string get_thread_id()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return std::to_string(std::stoull(ss.str()) % INT_MAX);
}

#endif