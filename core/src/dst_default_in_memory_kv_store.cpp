#include <dst_kv_store.h>
#include <map>
#include <string>

static std::map<std::string, std::string> kv;

static std::string EMPTY_STRING = "";

void dst_set_bool(const char *key, int value)
{
    kv[std::string(key)] = std::to_string(value);
}

int dst_get_bool_or_not_exist(const char *key)
{
    if (dst_exist(key))
    {
        return std::stoi(kv[std::string(key)]) == true;
    }
    return false;
}

int dst_exist(const char *key)
{
    if (kv.find(std::string(key)) != kv.end())
    {
        return true;
    }
    return false;
}

void dst_set(const char *key, const char *value)
{
    kv[std::string(key)] = std::string(value);
}

const char *dst_get(const char *key)
{
    if (dst_exist(key))
    {
        return kv[std::string(key)].c_str();
    }
    return EMPTY_STRING.c_str();
}