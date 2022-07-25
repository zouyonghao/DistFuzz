#ifndef __DST_COMMON_UTILS_H__
#define __DST_COMMON_UTILS_H__

#include <sstream>
#include <string>

#include <boost/process.hpp>

// static std::string get_thread_id()
// {
//     std::stringstream ss;
//     ss << std::this_thread::get_id();
//     return std::to_string(std::stoull(ss.str()) % INT_MAX);
// }

static bool retry_command(std::string command, int max_try_count)
{
    int count = 0;
    for (; count < max_try_count; count++)
    {
        boost::process::child c(command);
        c.wait();
        if (c.exit_code() == 0)
        {
            break;
        }
        usleep(500 * 1e3);
    }
    if (count >= max_try_count)
    {
        std::cerr << "Run init failed!\n";
        return false;
    }
    return true;
}

#endif