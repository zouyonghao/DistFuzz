#ifndef __DST_CONFIGURATION_GENERATOR_HEADER
#define __DST_CONFIGURATION_GENERATOR_HEADER

#include <string>
#include <cctype>

class ServerConfigurationGenerator
{
public:
    virtual std::string get_configure_string(uint32_t node_id, uint32_t node_count) = 0;
};

#endif // __DST_CONFIGURATION_GENERATOR_HEADER