#include <assert.h>
#include <dst_kv_store.h>
#include <string>

void test_get_set()
{
    assert(get("123")[0] == 0);
    set("123", "456");
    assert(std::string(get("123")) == "456");
}

void test_bool()
{
    assert(get_bool_or_not_exist("abc") == 0);
    set_bool("abc", true);
    assert(exist("abc") == true);
    assert(get_bool_or_not_exist("abc") == 1);
    assert(get_bool_or_not_exist("bcd") == 0);
}

int main(int argc, char const *argv[])
{
    test_get_set();
    test_bool();
    return 0;
}
