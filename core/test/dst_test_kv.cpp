#include <assert.h>
#include <dst_kv_store.h>
#include <string>

void test_get_set()
{
    assert(dst_get("123")[0] == 0);
    dst_set("123", "456");
    assert(std::string(dst_get("123")) == "456");
}

void test_bool()
{
    assert(dst_get_bool_or_not_exist("abc") == 0);
    dst_set_bool("abc", true);
    assert(dst_exist("abc") == true);
    assert(dst_get_bool_or_not_exist("abc") == 1);
    assert(dst_get_bool_or_not_exist("bcd") == 0);
}

int main(int argc, char const *argv[])
{
    test_get_set();
    test_bool();
    return 0;
}
