#ifndef DST_DEFAULT_CLIENT_OPERATOR_HEADER
#define DST_DEFAULT_CLIENT_OPERATOR_HEADER

#include <dst_configuration_generator.hpp>
#include <dst_event.h>
#include <dst_random.h>
#include <operator/dst_default_operator.hpp>
#include <util/common_uils.hpp>

#include <limits>
#include <map>
#include <sstream>

#include <boost/process.hpp>

enum OP_NAME
{
    OP_READ,
    OP_WRITE,
    OP_CAS
};

static const std::string OP_NAME_STR[] = {"read", "write", "cas"};

class ClientConfigurationGenerator
{
public:
    virtual std::string get_read_configure_string(uint32_t node_count) = 0;
    virtual std::string get_write_configure_string(uint32_t node_count, uint32_t value) = 0;
    virtual std::string get_cas_configure_string(uint32_t node_count, uint32_t old_value, uint32_t new_value) = 0;
};

/* The default client operator
 * Usually we support three operations: READ, WRITE, CAS for each system
 */
class DefaultClientOperator : public NormalOperator
{
protected:
    ClientConfigurationGenerator *configuration_generator;
    OP_NAME op_name;

public:
    DefaultClientOperator(OP_NAME _op_name, ClientConfigurationGenerator *_configuration_generator)
        : configuration_generator(_configuration_generator), op_name(_op_name)
    {
    }

    virtual bool _do();
};

#endif // DST_DEFAULT_CLIENT_OPERATOR_HEADER
