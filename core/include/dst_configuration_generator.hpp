#ifndef __DST_CONFIGURATION_GENERATOR_HEADER
#define __DST_CONFIGURATION_GENERATOR_HEADER

#include <cctype>
#include <string>

class ServerConfigurationGenerator
{
public:
    virtual std::string get_configure_string(uint32_t node_id, uint32_t node_count) = 0;

    virtual std::string get_data_folder(uint32_t node_id) { return ""; }

    /**
     * These files will not inject faults during fuzzing.
     * Example: a.txt,b.txt,...
     */
    virtual std::string get_no_fault_files(uint32_t node_id) { return ""; }
};

#endif // __DST_CONFIGURATION_GENERATOR_HEADER