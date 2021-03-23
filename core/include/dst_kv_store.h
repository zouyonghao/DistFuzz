#ifndef DST_KV_STORE_H
#define DST_KV_STORE_H

#ifdef __cplusplus
extern "C"
{
#endif

    extern void dst_set_bool(const char *key, int value);

    /**
     * @return 1 if the key exists and the value is 1
     * @return 0 if the key not exists or the value is 0
     */
    extern int dst_get_bool_or_not_exist(const char *key);

    extern int dst_exist(const char *key);

    extern void dst_set(const char *key, const char *value);

    extern const char *dst_get(const char *key);

    extern void dst_clear_all();

#ifdef __cplusplus
}
#endif

#endif // DST_KV_STORE_H
