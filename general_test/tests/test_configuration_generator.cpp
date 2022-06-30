#include <general_configuration_generator.hpp>

void test_configuration_generator()
{
    GeneralConfigurationGenerator gkcg;
    system("echo 'echo abc' > get_configure.sh && chmod +x get_configure.sh");
    assert(gkcg.get_configure_string(0, 3) == "abc");
    system("rm get_configure.sh");
}

void test_client_configuration_generator()
{
    GeneralClientConfigurationGenerator gccg;
    assert(gccg.get_configure_string(OP_READ, 3) == "./client.sh get");
    assert(gccg.get_configure_string(OP_WRITE, 3, 100) == "./client.sh set 100");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}