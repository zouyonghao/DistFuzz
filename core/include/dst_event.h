#ifndef DST_EVENT_HEADER
#define DST_EVENT_HEADER
#ifdef __cplusplus
extern "C"
{
#endif

    extern int __dst_event_trigger(const char *);

    extern int __dst_event_record(const char *);

#ifdef __cplusplus
}
#endif
#endif // DST_EVENT_HEADER